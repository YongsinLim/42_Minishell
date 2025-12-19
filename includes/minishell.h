/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 21:34:29 by jenjunn           #+#    #+#             */
/*   Updated: 2025/12/19 13:59:43 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include <readline/readline.h> // for readline
# include <readline/history.h> // for readline
# include <stdlib.h> // for malloc & free
# include <stdio.h> 


# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
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
void	add_to_history(char *command, t_history **history_list);
void	display_history(t_history *history_list);
void	free_history(t_history **history_list);


// ----- Token Functions -----
void	handle_word(char *line, int *i, t_token **tokens);
void	handle_redirection(char *line, int *i, t_token **tokens);
t_token	*tokenize(char *line);
t_token	*new_token(char *value, t_token_type type);
void	add_token(t_token **head, t_token *new_token);

#endif
