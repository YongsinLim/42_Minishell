/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 21:34:29 by jenjunn           #+#    #+#             */
/*   Updated: 2026/04/14 17:03:45 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include "signals.h"
# include <readline/readline.h> // for readline
# include <readline/history.h> // for readline, add_history
# include <stdlib.h> // for malloc, free, exit
# include <stdio.h> // for printf, perror
# include <unistd.h> // for access, write, execve, close, fork, dup, dup2, pipe,
					//	getcwd, chdir, isatty
# include <fcntl.h> // for open, O_RDONLY & O_WRONLY & O_TRUNC
# include <sys/wait.h> // for wait, waitpid
# include <limits.h> // for PATH_MAX
# include <dirent.h> // for opendir, readdir, closedir
# include <errno.h> // for errno






# include <sys/stat.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <term.h>

# define SHELL_SUCCESS 0
# define SHELL_FAILURE 1
# define SYNTAX_ERROR 2
# define TRUE 1
# define FALSE 0
# define CMD_NOT_FOUND 127
# define ACCESS_PERMITTED 0
# define ERROR -1


typedef struct s_env
{
	char			*key;	// e.g., "USER"
	char			*value; // e.g., "me"
	struct s_env	*next;
}				t_env;

typedef struct s_history
{
	char				*command;
	struct s_history	*next;
}				t_history;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_L_PAREN,
	TOKEN_R_PAREN
}			t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				has_quotes;
	int				has_wildcard;
	struct s_token	*next;
}				t_token;

typedef enum e_ast_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}			t_ast_node_type;

typedef struct s_redir
{
	t_token_type	type;	// Hold TOKEN_REDIRECT_OUT, TOKEN_APPEND,
							// TOKEN_REDIRECT_IN
	char			*file;
	struct s_redir	*next;
}				t_redir;

typedef struct s_command
{
	char		**argv;	// Hold command (eg. echo)
	t_redir		*redirs;
	char		*heredoc_delimiter;
	int			heredoc_is_quoted;
	int			heredoc_fd;
}				t_command;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_command			*command;
}				t_ast_node;

typedef struct s_minishell
{
	t_env		*env_list;
	char		*input;
	t_history	*history_list;
	t_token		*tokens;
	t_ast_node	*ast;
	int			last_exit_status;
}				t_minishell;

typedef struct s_token_to_word
{
	t_minishell	*minishell;
	int			is_quoted_string;
	int			is_contain_wildcard;
	int			is_disabled_expand;
}				t_token_to_word;

// ----- Main Functions -----
int				read_and_prepare_input(t_minishell *minishell, int interactive);
int				is_all_whitespace(char *str);
int				build_tokens(t_minishell *minishell);

void			cleanup_and_exit(t_minishell *minishell, int exit_status);

// ----- Init Functions -----
void			init_minishell(t_minishell *minishell, char **envp);
t_env			*init_env(char **envp);
void			increment_shlvl(t_minishell *minishell);

// ----- Env_Utils Functions -----
t_env			*env_from_str(char *str);
t_env			*new_env_node(char *key, char *value);
void			update_env(char *key, char *value, t_minishell *minishell);
char			*get_var_value(char *var_name, t_minishell *minishell);

// ----- Unclosed Quotes Functions -----
char			*check_unclosed_quotes(char *input, int is_interactive);
int				has_unclosed_quotes(char *str);
char			*strjoin_with_nextline(char *s1, char *s2);

// ----- History Functions -----
void			add_to_history(char *command, t_history **history_list);
void			free_history(t_history **history_list);

// ----- Token_Helper Functions -----
void			add_token(t_token **head, t_token *new_token);
t_token			*make_token(char *value, int *i);
t_token			*new_token(char *value, t_token_type type, int has_quotes);

// ----- Token Functions -----
t_token			*tokenize(char *line, t_minishell *minishell);
void			skip_invalid_char(char *line, int *i);
void			add_redirection_token(char *line, int *i, t_token **tokens);

// ----- Token_Words Functions -----
char			*handle_quoted_string(char *line, int *i,
					t_minishell *minishell, int disable_expand);
void			escape_whitespace(char *str);
char			*get_unquoted_segment(char *line, int *i,
					t_minishell *minishell, int disable_expand);
char			*strjoin_free(char *full_word, char *segment);
void			create_and_add_word_token(char *full_word,
					t_token_to_word *word, t_token **tokens);

// ----- Token_Words_Main Functions -----
int				handle_word(char *line, int *i, t_token **tokens,
					t_minishell *minishell);
int				previous_token_is_heredoc(t_token *tokens);
char			*expand_tilde_prefix(char *line, int *i, t_minishell *minishell,
					int disable_expand);
int				is_separator(char c);
char			*get_segment_check_wildcard(char *line, int *i,
					t_token_to_word *word, char *full_word);

// ----- Expansion Functions -----
char			*expand_variable(char *str, t_minishell *minishell);
char			*handle_dollar_sign(char *str, int *i_ptr, char *final_str,
					t_minishell *minishell);
int				get_var_name_len(char *str, char **var_name_ptr);
char			*append_char(char *s1, char c);
char			*append_segment(char *final_str, char *str, int *i);

// ----- Free Functions -----
void			free_tokens(t_token **tokens);
void			free_env_list(t_env *env);
void			free_ast(t_ast_node **ast_ptr);
void			free_command(t_command *cmd);
void			free_array_str(char **array);

// ----- Parse Subshell Or Command Functions -----
t_ast_node		*parse_command_or_subshell(t_token **tokens);
t_ast_node		*parse_subshell(t_token **tokens);
t_ast_node		*parse_command(t_token **tokens);
t_ast_node		*create_new_ast_node(t_ast_node_type type);

// ----- Parse_Helper Functions -----
t_command		*init_cmd(void);
int				is_not_logical_operator(t_token *token);
int				is_redirection_token(t_token *token);
int				is_whitespace(char c);
void			*parse_error_cleanup(t_list **argv_list, t_command *cmd,
					char *msg);

// ----- Parse_Redirection Functions -----
int				parse_redirection(t_token **tokens, t_command *cmd);
int				set_redirection_file(t_token *token, t_command *command);
int				get_redirection_file(t_token **target, t_token *token);
void			restore_quoted_whitespaces(char *str);
int				add_redir(t_command *cmd, t_token_type type, char *file);

// ----- General_Helper Functions -----
char			**convert_list_to_str_array(t_list *argv_list);

// ----- Parse_One_Command Functions -----
t_command		*parse_one_command(t_token **tokens_ptr);
int				tokens_to_cmd(t_token **token, t_list **argv_list);
int				build_word_field(t_list	**temp_split, t_token *token);
int				add_argv_value(t_list **argv_list, char *value);
int				split_unquoted_word_to_argv(char *value, t_list **argv_list);

// ----- Wildcard Functions -----
void			expand_wildcard(char *pattern, t_list **argv_list);
int				match_pattern(char *pattern, char *filename);
int				add_arg_node(t_list **argv_list, char *value);
void			sort_argv_list(t_list **argv_list);
int				compare_filename(char *a, char *b);

// ----- Parse Functions -----
t_ast_node		*parse(t_token **tokens);
t_ast_node		*parse_pipeline(t_token **tokens);
t_ast_node_type	set_operator_type(t_token **tokens);

// ----- Redirection Functions -----
int				redirect_input(t_command *cmd);
int				redirect_open_error(char *file);
int				redirect_output(t_command *cmd);

// ----- Builtins Functions -----
int				is_builtin(char *cmd);
int				handle_builtin_execution(t_ast_node *ast,
					t_minishell *minishell);
int				execute_builtin(char **argv, t_minishell *minishell);

// ----- Builtin - Echo -----
int				ft_echo(char **argv);
void			print_arguments(char **argv, int i, int n_flag);

// ----- Builtin - Cd -----
int				ft_cd(char **argv, t_minishell *minishell);
char			*get_target_path(char **argv, t_minishell *minishell);

// ----- Report Error Functions -----
void			report_error(char *msg, char *param);
void			report_chdir_error(char *path);

// ----- Builtin - Pwd -----
int				ft_pwd(void);

// ----- Builtin - Export (Env Sort) -----
int				count_env_nodes(t_env *env_list);
void			sort_env_parts(t_env **arr, int count);
int				env_key_cmp(t_env *a, t_env *b);
void			swap_env_ptr(t_env **a, t_env **b);
// -----------------------------------------------------------------------------










void			print_export_node(t_env *node);

// ----- Builtin - Export -----
int				ft_export(char **argv, t_minishell *minishell);
int				print_export(t_env *env);
int				export_invalid_option(char *arg);
int				check_valid_identifier(char *arg);
void			process_export_arg(char *arg, t_minishell *minishell);

// ----- Debug Functions -----
void			print_indent(int level);
void			print_ast(t_ast_node *node, int level);

// ----- Main
void			execution(t_minishell *minishell);

// ----- Env_Convert Functions -----
char			**env_list_to_array(t_env *env_list);

// ----- Builtin - Unset -----
int				check_valid_unset_identifier(char *arg);
void			remove_env_node(t_env **env_head, char *key);
int				ft_unset(char **argv, t_minishell *minishell);

// ----- Builtin - Env -----
int				ft_env(char **argv, t_minishell *minishell);

// ----- Builtin - Exit -----
int				ft_is_numeric(char *str);
int				ft_exit(char **argv, t_minishell *minishell);

// ----- Heredoc Functions -----
// int				heredocs(t_ast_node *ast, t_minishell *minishell);
// void			heredoc_read_loop(t_command *cmd, t_minishell *minishell,
//					int write_fd);
// int				process_heredoc(t_command *cmd, t_minishell *minishell);

void			heredocs(t_ast_node *ast, t_minishell *minishell);
void			process_heredoc(t_command *cmd, t_minishell *minishell);
char			*read_heredoc_line_simple(void);
void			heredoc_child_process(t_command *cmd, t_minishell *minishell,
					int write_fd);
void			process_heredoc_noninteractive(t_command *cmd,
					t_minishell *minishell, int *pipe_fd);

// ----- Execute Functions -----
int				execute_ast(t_ast_node *ast, t_minishell *minishell);
int				execute_simple_command(t_ast_node *ast, t_minishell *minishell);

int				execute_pipe(t_ast_node *ast, t_minishell *minishell);
int				execute_subshell(t_ast_node *ast, t_minishell *minishell);

// ----- Execute Path Functions -----
void			execute(char **cmd_array, t_minishell *minishell);
char			*build_path(char *cmd, t_minishell *minishell);
char			*construct_full_path(t_env *env_list, char *command);
char			**get_path(t_env *env_list);
int				is_directory_path(char *path);
char			*search_path(char **path_dir, char *command);

// ----- Wait Child Functions -----
int				wait_for_children(pid_t last_pid);


void			error_exit(char *error_msg);
char			*exit_status(t_minishell *minishell, char *cmd_name,
					char *message, int exit_status);



void			execute_pipe_left(t_ast_node *ast, t_minishell *minishell,
					int *pipe_fd);
void			execute_pipe_right(t_ast_node *ast, t_minishell *minishell,
					int *pipe_fd);

// ----- History
int				ft_history(char **argv, t_history *history_list);

#endif
