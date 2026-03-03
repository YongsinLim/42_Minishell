/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 21:34:29 by jenjunn           #+#    #+#             */
/*   Updated: 2026/03/03 19:11:00 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <readline/readline.h> // for readline
# include <readline/history.h> // for readline, add_history
# include <stdlib.h> // for malloc, free, exit
# include <stdio.h> // for printf, perror
# include <unistd.h> // for access, write, execve, close, fork, dup, dup2, pipe, getcwd, chdir
# include <fcntl.h> // for open, O_RDONLY & O_WRONLY & O_TRUNC
# include <sys/wait.h> // for wait, waitpid
# include <limits.h> // for PATH_MAX



# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
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

typedef struct s_history
{
	char				*command;
	struct s_history	*next;
}				t_history;

typedef struct s_env
{
	char            *key;   // e.g., "USER"
	char            *value; // e.g., "me"
	struct s_env    *next;
}				t_env;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_QUOTED_STRING,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_L_PAREN,
	TOKEN_R_PAREN
}			t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
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

typedef struct s_command
{
	char				**argv;
	char				*input_file;
	char				*output_file;
	int					is_append;
	char				*heredoc_delimiter;
	int					heredoc_is_quoted;
	int					heredoc_fd;
}				t_command;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_command			*command;
}						t_ast_node;

typedef struct s_minishell
{
	t_history	*history_list;
	t_env		*env_list;
	t_token		*tokens;
	t_ast_node	*ast;
	char		*input;
	int			last_exit_status;
}				t_minishell;

// ----- Debug Functions -----
void			print_indent(int level);
void			print_ast(t_ast_node *node, int level);

// ----- Main Functions -----
void			execution(t_minishell *minishell);
void			cleanup_and_exit(t_minishell *minishell, int exit_status);


// ----- Env_Utils Functions -----
t_env			*new_env_node(char *key, char *value);
t_env			*init_env(char **envp);


int				count_env_nodes(t_env *env_list);
char			**env_list_to_array(t_env *env_list);







// ----- Builtins Functions -----
int				is_builtin(char *cmd);
int				execute_builtin(char **argv, t_minishell *minishell);

// ----- Builtin - Export -----






// ----- Builtin - Cd -----
char			*get_target_path(char **argv, t_minishell *minishell);
void			update_env(char *key, char *value, t_minishell *minishell);
int				ft_cd(char **argv, t_minishell *minishell);

// ----- Builtin - Unset -----
int				is_valid_identifier(char *str);
void			remove_env_node(t_env **env_head, char *key);
int				ft_unset(char **argv, t_minishell *minishell);

// ----- Builtin - Echo -----
void			print_arguments(char **argv, int i, int n_flag);
int				ft_echo(char **argv);

// ----- Builtin - Pwd -----
int				ft_pwd(void);

// ----- Builtin - Env -----
int				ft_env(t_minishell *minishell);

// ----- Builtin - Exit -----
int				ft_is_numeric(char *str);
int				ft_exit(char **argv, t_minishell *minishell);

// ----- Heredoc Functions -----
void			heredocs(t_ast_node *ast, t_minishell *minishell);
void			process_heredoc(t_command *cmd, t_minishell *minishell);
char			*verify_expand_heredoc(t_command *cmd, t_minishell *minishell, char *line,
					char *limiter);

// ----- Execute Functions -----
int				execute_ast(t_ast_node *ast, t_minishell *minishell);
int				execute_simple_command(t_ast_node *ast, t_minishell *minishell);
int				exec_pipe(t_ast_node *ast, t_minishell *minishell);
int				exec_subshell(t_ast_node *ast, t_minishell *minishell);

// ----- Execute Path Functions -----
void			execute(char **cmd_array, t_minishell *minishell);
char			*construct_full_path(t_env *env_list, char *command);
char			**get_path(t_env *env_list);
char			*search_path(char **path_dir, char *command);
void			report_error(char *msg, char *param);

// ----- Wait Child Functions -----
int				wait_for_children(pid_t last_pid);
void			error_exit(char *error_msg);

// ----- Redirection Functions -----
void			redirect_input(t_command *cmd);
void			redirect_output(t_command *cmd);
void			execute_pipe_left(t_ast_node *ast, t_minishell *minishell, int *pipe_fd);
void			execute_pipe_right(t_ast_node *ast, t_minishell *minishell, int *pipe_fd);

// ----- Parse Functions -----
t_ast_node		*parse(t_token **tokens);
t_ast_node		*parse_pipeline(t_token **tokens);
t_ast_node		*create_new_ast_node(t_ast_node_type type);
t_ast_node_type	set_operator_type(t_token **tokens);

t_ast_node		*parse_command_or_subshell(t_token **tokens);
t_ast_node		*parse_subshell(t_token **tokens);
t_ast_node		*parse_simple_command(t_token **tokens);

t_command		*parse_one_command(t_token **tokens_ptr);
int				tokens_to_cmd(t_token **token_ptr, t_command *cmd,
					t_list **argv_list);
int				parse_redirection(t_token **tokens, t_command *cmd);
t_token			*token_redirection(t_token *token, t_command *command);
char			**to_str_array(t_list *argv_list);

// ----- Parse_Helper Functions -----
t_command		*init_cmd(void);
void			*parse_error_cleanup(t_list **argv_list, t_command *cmd,
					char *msg);

// ----- Free Functions -----
void			free_tokens(t_token **tokens);
void			free_array_str(char **array);
void			free_command(t_command *cmd);
void			free_ast(t_ast_node **ast_ptr);
void			free_env_list(t_env *env);

// ----- History Functions -----
void			add_to_history(char *command, t_history **history_list);
int				ft_history(char **argv, t_history *history_list);
void			free_history(t_history **history_list);

// ----- Expansion Functions -----
char			*expand_variable(char *str, t_minishell *minishell);
char			*handle_dollar_sign(char *str, int *i_ptr, char *final_str,
			                        t_minishell *minishell);
char			*append_segment(char *final_str, char *str, int *i);
int				get_var_name_len(char *str, char **var_name_ptr);
char			*get_var_value(char *var_name, t_minishell *minishell);
char			*append_char(char *s1, char c);


// ----- Token Functions -----
void			skip_spaces(char *line, int *i);
void			add_redirection_token(char *line, int *i, t_token **tokens);
t_token			*tokenize(char *line, t_minishell *minishell);
t_token			*new_token(char *value, t_token_type type);
t_token			*make_token(char *value, int *i);
void			add_token(t_token **head, t_token *new_token);
int				handle_quoted_string(char *line, int *i, t_token **tokens,
					t_minishell *minishell);
int				is_separator(char c);
char			*expand_tilde(char *str, t_minishell *minishell);
void			handle_word(char *line, int *i, t_token **tokens, t_minishell *minishell);

#endif
