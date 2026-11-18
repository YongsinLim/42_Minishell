# Minishell — Detailed Architecture & Execution Workflow

This document explains the complete lifecycle of a command in **Minishell**, from startup to tokenization, AST construction, heredoc preprocessing, execution, signal handling, and cleanup, with exact references to the codebase.

---

## 1. High-Level Pipeline

Every command line passes through five sequential stages inside the main loop in [`src/main.c`](src/main.c#L55-L80):

```text
  readline / get_next_line
            │
            ▼
  1. Input Preparation & History   (read_and_prepare_input)
            │
            ▼
  2. Lexing & Expansion            (tokenize -> t_token list)
            │
            ▼
  3. Recursive-Descent Parsing     (parse -> t_ast_node tree)
            │
            ▼
  4. Heredoc Preprocessing         (heredocs -> pipe FDs in t_redir)
            │
            ▼
  5. AST Execution & Cleanup       (execute_ast -> free_tokens / free_ast)
```

---

## 2. Core Data Structures ([`includes/minishell.h`](includes/minishell.h#L43-L129))

| Struct | Purpose | Key Fields |
| :--- | :--- | :--- |
| `t_minishell` | Global shell state (passed through all stages) | `env_list`, `input`, `history_list`, `tokens`, `ast`, `last_exit_status` |
| `t_env` | Linked list of environment variables | `key`, `value`, `next` |
| `t_token` | Lexer output node | `value`, `type` (`e_token_type`), `has_quotes`, `has_wildcard`, `next` |
| `t_redir` | Single redirection or heredoc entry | `type`, `file` (or delimiter), `is_quoted`, `heredoc_fd`, `next` |
| `t_command` | Simple command or subshell redirections | `argv` (`char **`), `redirs` (`t_redir *`) |
| `t_ast_node` | Binary AST node (`COMMAND`, `PIPE`, `AND`, `OR`, `SUBSHELL`) | `type`, `left`, `right`, `command` |

Only one global variable exists in the entire project, as required by the 42 subject:
- `extern volatile sig_atomic_t g_signal;` declared in [`includes/signals.h`](includes/signals.h#L20) and defined in [`src/main.c`](src/main.c#L15). It stores only the received signal number (`SIGINT`).

---

## 3. Stage 0 — Initialization & Main Loop

### Startup ([`src/initialization/init.c`](src/initialization/init.c#L15-L73))
1. `init_minishell()` initializes `t_minishell` fields to `NULL`/`0` and calls `init_env(envp)` to copy `envp` into a `t_env` linked list.
2. `update_env("OLDPWD", NULL, minishell)` registers `OLDPWD` without a value (so `export` lists it, while `env` omits it until `cd` sets it).
3. `increment_shlvl()` reads `SHLVL` and increments it by `1` (or sets `"1"` if unset).
4. `init_signals_prompt()` ([`src/signals/signal_init.c`](src/signals/signal_init.c#L15-L19)) installs `sigint_handler_prompt` for `SIGINT` and `SIG_IGN` for `SIGQUIT`.
5. `interactive = isatty(STDIN_FILENO)` determines whether input comes from a terminal (`readline`) or a pipe/script (`get_next_line`).

### Input Preparation ([`src/main.c`](src/main.c#L84-L110))
1. `read_and_prepare_input()` reads a line via `readline("Minishell > ")` (interactive) or `get_next_line(STDIN_FILENO)` (non-interactive).
2. If `g_signal == SIGINT`, it updates `minishell->last_exit_status = 130` and resets `g_signal = 0` so `$?` reflects Ctrl+C.
3. If `raw_line == NULL` (EOF / Ctrl+D), it prints `"exit\n"` to `stderr` in interactive mode and breaks out of the main loop.
4. `ft_strtrim(raw_line, "\r\n")` strips line endings and `save_input_to_history()` ([`src/history/history.c`](src/history/history.c#L15-L37)) records non-whitespace lines via `add_history()` and `history_list`.

---

## 4. Stage 1 — Lexing, Quoting & Variable Expansion

Implemented in `src/tokens/` and `src/unclosed_quotes/`:

### Unclosed Quote Check ([`src/unclosed_quotes/unclosed_quotes.c`](src/unclosed_quotes/unclosed_quotes.c#L42-L60))
- Before tokenizing, `tokenize()` ([`src/tokens/main_tokenising.c`](src/tokens/main_tokenising.c#L25-L53)) calls `has_unclosed_quotes(line)`.
- If a single `'` or double `"` quote is unclosed, it prints `minishell: syntax error: unclosed quote` to `stderr`, sets `last_exit_status = SYNTAX_ERROR` (`2`), and aborts tokenization (per subject: *"Not interpret unclosed quotes"*).

### Tokenizing Loop ([`src/tokens/main_tokenising.c`](src/tokens/main_tokenising.c#L25-L53))
1. `skip_invalid_char()` skips spaces, tabs, and `$` immediately preceding quotes (`$"..."`, `$'...'`).
2. A stray `&` (not followed by `&`) triggers `syntax_error_token()` (`SYNTAX_ERROR`).
3. Operators `&&`, `||`, `|`, `(`, `)` are converted by `make_token()` ([`src/tokens/token_helper.c`](src/tokens/token_helper.c#L30-L51)).
4. Redirections `<<`, `<`, `>>`, `>`, `>|` are converted by `add_redirection_token()` ([`src/tokens/main_tokenising.c`](src/tokens/main_tokenising.c#L55-L85)).
5. Anything else is parsed as a word by `handle_word()` ([`src/tokens/token_words_main.c`](src/tokens/token_words_main.c#L21-L42)):
   - `previous_token_is_heredoc()` disables `$` and `~` expansion for heredoc delimiters.
   - `expand_tilde_prefix()` expands a leading `~` (when followed by separator, `/`, or `\0`) to `$HOME`.
   - `get_segment_check_wildcard()` loops until a separator, calling either:
     - `handle_quoted_string()` ([`src/tokens/token_words.c`](src/tokens/token_words.c#L16-L44)): expands `$VAR` / `$?` only inside `"..."` (never `'...'`), then calls `escape_whitespace()` to encode literal spaces (`\x1A`), tabs (`\x1B`), newlines (`\x1C`), `*` (`\x1D`), and `?` (`\x1E`) so later field-splitting and wildcard matching do not treat quoted characters as delimiters or globs.
     - `get_unquoted_segment()` ([`src/tokens/token_words.c`](src/tokens/token_words.c#L67-L85)): expands `$VAR` / `$?` via `expand_variable()` ([`src/tokens/expansion.c`](src/tokens/expansion.c#L15-L60)) and flags `is_contain_wildcard = TRUE` if `*` or `?` appears unquoted.

---

## 5. Stage 2 — AST Parsing (`src/ast_build/`)

The parser builds a binary tree respecting Bash operator precedence:

```text
  Precedence (lowest -> highest):
    1. && / ||          parse()                    (left-associative)
    2. |                parse_pipeline()           (left-associative)
    3. (...) / command  parse_command_or_subshell()
```

### Grammar & Precedence
- `parse()` ([`src/ast_build/parse.c`](src/ast_build/parse.c#L15-L38)): calls `parse_pipeline()`, then loops over `TOKEN_AND` (`&&`) and `TOKEN_OR` (`||`).
- `parse_pipeline()` ([`src/ast_build/parse.c`](src/ast_build/parse.c#L41-L66)): calls `parse_command_or_subshell()`, then loops over `TOKEN_PIPE` (`|`).
- `parse_command_or_subshell()` ([`src/ast_build/command_or_subshell.c`](src/ast_build/command_or_subshell.c#L15-L23)):
  - `TOKEN_L_PAREN` → `parse_subshell()`: consumes `(`, recursively calls `parse()`, expects `TOKEN_R_PAREN` `)`, creates `NODE_SUBSHELL`, and parses any trailing redirections on the subshell (e.g., `(ls) > out`).
  - Otherwise → `parse_command()`: rejects unexpected leading operators (`|`, `&&`, `||`, `)`) and builds `NODE_COMMAND` via `parse_one_command()`.
- Top-level trailing token check in `execution()` ([`src/main.c`](src/main.c#L151-L157)): if `parse()` returns an AST while unconsumed tokens remain (e.g., `echo hi )`), it reports a syntax error (`SYNTAX_ERROR`) and frees the AST.

### Command Building, Word Splitting & Wildcards
- `parse_one_command()` ([`src/ast_build/parse_one_command.c`](src/ast_build/parse_one_command.c#L29-L53)) iterates until a logical operator or parenthesis:
  - Redirections (`<`, `>`, `>>`, `<<`) are appended in left-to-right order to `cmd->redirs` via `parse_redirection()` ([`src/ast_build/parse_redirection.c`](src/ast_build/parse_redirection.c#L15-L62)), with `is_quoted` saved on `TOKEN_HEREDOC` nodes.
  - Words go through `tokens_to_cmd()` ([`src/ast_build/tokens_to_cmd.c`](src/ast_build/tokens_to_cmd.c#L15-L70)):
    1. `build_word_field()` preserves quoted empty strings (`""`) and splits unquoted whitespace via `split_unquoted_word_to_argv()`, restoring `\x1A`/`\x1B`/`\x1C` back to `' '`/`'\t'`/`'\n'`.
    2. If `token->has_wildcard` is set, `expand_wildcard()` ([`src/wildcard/wildcard.c`](src/wildcard/wildcard.c#L34-L61)) opens `.` with `opendir(".")`, matches entries with `match_pattern()` (skipping `.`-files unless the pattern starts with `.`, and matching `\x1D`/`\x1E` only against literal `*`/`?`), and sorts matches lexicographically via `sort_argv_list()`.
    3. `restore_quoted_wildcards()` converts `\x1D`/`\x1E` back to literal `*`/`?`.
  - `convert_list_to_str_array()` converts the `t_list` into `cmd->argv` (`char **`).

---

## 6. Stage 3 — Heredoc Preprocessing (`src/ast_execute/heredoc*.c`)

Before executing any command in the AST, `heredocs()` ([`src/ast_execute/heredoc.c`](src/ast_execute/heredoc.c#L21-L65)) traverses the entire tree left-to-right and processes every `TOKEN_HEREDOC` in `cmd->redirs`:

1. Opens a `pipe(pipe_fd)`.
2. **Interactive mode** (`process_heredoc_interactive()` in [`src/ast_execute/heredoc_interactive.c`](src/ast_execute/heredoc_interactive.c#L15-L72)):
   - Forks a child process so `Ctrl+C` (`SIGINT` with `SA_RESTART` disabled in `setup_heredoc_signals()`) interrupts only the heredoc read and returns exit status `130`.
   - `read_heredoc_line()` reads dynamically from `stdin` using `append_char()` (no fixed buffer limit).
   - If the child exits with `0`, `pipe_fd[0]` is stored in `redir->heredoc_fd`.
3. **Non-interactive mode** (`process_heredoc_noninteractive()` in [`src/ast_execute/heredoc_noninteractive.c`](src/ast_execute/heredoc_noninteractive.c#L15-L43)):
   - Reads lines with `get_next_line(STDIN_FILENO)` and writes to `pipe_fd[1]`.
4. `process_heredoc_line()` expands `$VAR` / `$?` on each line only when `redir->is_quoted == FALSE` (unquoted delimiter).

---

## 7. Stage 4 — AST Execution (`src/ast_execute/` & `src/builtins/`)

`execute_ast()` ([`src/ast_execute/ast_executor.c`](src/ast_execute/ast_executor.c#L15-L36)) dispatches by node type:

- **`NODE_AND` / `NODE_OR`**: executes `ast->left`, then short-circuits or executes `ast->right` based on `status == SHELL_SUCCESS`.
- **`NODE_PIPE`** (`execute_pipe()`): creates a `pipe(pipe_fd)`, forks `pid_left` (`dup2(pipe_fd[1], STDOUT_FILENO)`) and `pid_right` (`dup2(pipe_fd[0], STDIN_FILENO)`), closes both pipe ends in the parent, and waits via `wait_pipeline_status(pid_right)` so the pipeline returns the rightmost command's exit status.
- **`NODE_SUBSHELL`** (`execute_subshell()`): forks a child, applies subshell redirections (`apply_redirections()`), recursively runs `execute_ast(ast->left, minishell)`, and calls `cleanup_and_exit()`.
- **`NODE_COMMAND`** (`execute_simple_command()` in [`src/ast_execute/execute_simple_cmd.c`](src/ast_execute/execute_simple_cmd.c#L27-L44)):
  - **Empty command (`argv[0] == NULL`)**: `handle_empty_cmd_redirs()` backs up `STDIN_FILENO`/`STDOUT_FILENO`, runs `apply_redirections()`, and restores the FDs.
  - **Builtin (`echo`, `cd`, `pwd`, `export`, `unset`, `env` with no args, `exit`, `history`)**: `handle_builtin_execution()` ([`src/builtins/builtins.c`](src/builtins/builtins.c#L38-L61)) backs up `stdin`/`stdout`, applies redirections, executes the builtin in the current shell process, and restores the FDs.
  - **External command**: `run_bash_cmd()` forks a child, restores default signals (`init_signals_child()`), applies redirections (`apply_redirections()` in left-to-right order over `cmd->redirs`), resolves the executable via `build_path()` ([`src/ast_execute/build_path.c`](src/ast_execute/build_path.c#L24-L74)), updates `$_`, converts `env_list` to `char **`, and calls `execve()`. The parent closes heredoc FDs via `close_cmd_heredocs()` and waits with `wait_for_children()`.

---

## 8. Stage 5 — Memory & Exit Cleanup

- **Per-command cleanup** (`execution()` in [`src/main.c`](src/main.c#L162-L165)): frees `minishell->input`, `minishell->tokens` (`free_tokens()`), and `minishell->ast` (`free_ast()` -> `free_command()`, which also closes any remaining `heredoc_fd`).
- **Shell exit cleanup** (`cleanup_and_exit()` in [`src/free_memory/cleanup_exit.c`](src/free_memory/cleanup_exit.c#L25-L34)): frees `env_list`, `input`, `history_list`, `tokens`, `ast`, calls `rl_clear_history()`, and exits with `exit_status`.
