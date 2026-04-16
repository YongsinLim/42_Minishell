# Welcome to Minishell

This project has been created as part of the 42 curriculum by jenlee and yolim
A 42 school team project implementing a basic POSIX-compliant shell in C, mimicking Bash Version 5.1.16.

**Developed in collaboration with [ABCSoup](https://github.com/DaSoupOfABC)**

---

## Overview

Minishell is a command-line interpreter that provides a basic shell environment supporting command execution, input parsing, redirections, pipes, environment variables, built-in commands, and signal handling. It replicates core Bash functionality with a focus on learning shell architecture and systems programming.

---

## Features

- **Prompt display** — Displays an interactive prompt while waiting for commands
- **Command history** — Navigate and reuse previous commands using arrow keys
- **Command execution** — Search and launch executables from PATH or use relative/absolute paths
- **Quoting**
    - Single quotes `'` prevent all meta-character interpretation
    - Double quotes `"` prevent meta-character interpretation except for `$` (variable expansion)
- **Redirections**
    - Input: `<` (read from file)
    - Output: `>` (write to file)
    - Append: `>>` (append to file)
    - Here-document: `<<` (read until delimiter)
- **Pipes** — Chain commands with `|` to connect output of one command to input of the next
- **Environment variables** — Expand variables with `$VARIABLE_NAME` syntax
- **Special variables** — `$?` expands to the exit status of the most recently executed command
- **Signal handling**
    - `Ctrl+C` (SIGINT) — Interrupt current command
    - `Ctrl+D` (EOF) — Exit the shell
    - `Ctrl+\` (SIGQUIT) — Quit with core dump
- **Built-in commands** — `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit` with proper options
- **Logical operators** — `&&` and `||` with parenthesis for operation priorities
- **Wildcard expansion** — `*` expansion for files in the current directory

---

## Requirements

- **Compiler:** GCC or Clang
- **Libraries:** GNU Readline library (`libreadline-dev` on Ubuntu/Debian, `readline` on macOS via Homebrew)
- **OS:** POSIX-compliant system (Linux/macOS)
- **C Standard:** C99 or later

---

## Usage

To compile the Minishell program, use the provided Makefile by running `make` in the project directory. This will generate an executable named `minishell`.

```
make
```

To run the Minishell program, simply execute the generated `minishell` binary.

```
./minishell
```
Once running, you can enter commands and interact with the shell.

To clean the Minishell files
```
make clean      # Remove object files
make fclean     # Remove all generated files including binary
make re         # Rebuild everything
```
---

## Limitations

- Minishell does not interpret special characters, such as backslashes `\` or semicolons `;` as escape characters or command separators
- Memory leaks: The readline() library may cause memory leaks (external library limitation), but all code written for this project is leak-free.

--- 

## Status

Validated: xx/04/2026.

Grade: ?/125.

---

### Used tests
- Tester Repository: https://github.com/zstenger93/42_minishell_tester
