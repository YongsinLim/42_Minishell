/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:41:50 by yolim             #+#    #+#             */
/*   Updated: 2026/01/02 18:49:29 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_heredocs(t_command *pipeline)
{
	t_command	*current_cmd;

	current_cmd = pipeline;
	while (current_cmd != NULL)
	{
		if (current_cmd->heredoc_delimiter != NULL)
			process_heredoc(current_cmd);
		current_cmd = current_cmd->next;
	}
}

void	process_heredoc(t_command *cmd)
{
	int		pipe_fd[2];
	char	*line;
	char	*limiter;
	int		len;

	if (pipe(pipe_fd) == -1)
		error_exit("Heredoc pipe failed");
	limiter = cmd->heredoc_delimiter;
	len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(0);
		if (!line || (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n'))
		{
			if (line)
				free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	cmd->heredoc_fd = pipe_fd[0];
}

int	count_words(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_APPEND
			|| tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next;
			if (tokens == NULL)
				break ;
		}
		else if (tokens->type == TOKEN_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
