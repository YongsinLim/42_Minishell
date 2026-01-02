/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 21:34:29 by jenjunn           #+#    #+#             */
/*   Updated: 2026/01/02 19:00:14 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <readline/readline.h> // for readline
# include <readline/history.h> // for readline
# include <stdlib.h> // for malloc, free, exit
# include <stdio.h> // for printf, perror
# include <unistd.h> // for access, write, execve, open, close, fork
# include <fcntl.h> // for O_RDONLY & O_WRONLY & O_TRUNC
# include <sys/wait.h> // for wait, waitpid



# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <term.h>

typedef struct s_history
{
	char				*command;
	struct s_history	*next;
}				t_history;

typedef struct s_command
{
	char				**argv;
	char				*input_file;
	char				*output_file;
	int					is_append;
	char				*heredoc_delimiter;
	int					heredoc_fd;
	struct s_command	*next;
}			t_command;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}			t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_vars
{
	t_token			token;
}				t_vars;

// ----- History Functions -----
void		add_to_history(char *command, t_history **history_list);
void		display_history(t_history *history_list);
void		free_history(t_history **history_list);

// ----- Parse Functions -----
t_command	*parse(t_token *tokens);
t_command	*assign_next_cmd(t_command *current, t_command *new_cmd);
t_command	*parse_one_command(t_token **tokens_ptr);
t_command	*initialise_command_list(int words_count);
t_token		*token_redirection(t_token *token, t_command *command_list);

// ----- Heredoc Functions -----
void		handle_heredocs(t_command *pipeline);
void		process_heredoc(t_command *cmd);
int			count_words(t_token *tokens);

// ----- Executor Functions -----
int			execute_pipeline(t_command *pipeline, char *envp[]);
void		redirect_input(int prev_pipe_read_end, t_command *cmd);
void		redirect_output(t_command *cmd, int *pipe_fd);
void		parent_pipe_handler(int *previous_pipe_read_end,
				t_command *pipeline, int pipe_fd[2]);
int			cleanup_and_wait(int previous_pipe_read_end, pid_t fork_pid_result);

// ----- Execute Path Functions -----
void		execute(char **cmd_array, char **envp);
char		*construct_full_path(char *envp[], char *command);
char		**get_path(char *envp[]);
char		*search_path(char **path_dir, char *command);
void		report_error(char *msg, char *param, char **free_me, int exit_code);

// ----- Wait Child Functions -----
int			wait_for_children(pid_t last_pid);
void		error_exit(char *error_msg);

// ----- Free Functions -----
void		free_tokens(t_token **tokens);
void		free_pipeline(t_command **pipeline);
void		free_array_str(char **array);

// ----- Token Functions -----
void		handle_word(char *line, int *i, t_token **tokens);
void		handle_redirection(char *line, int *i, t_token **tokens);
t_token		*tokenize(char *line);
t_token		*new_token(char *value, t_token_type type);
void		add_token(t_token **head, t_token *new_token);

#endif
