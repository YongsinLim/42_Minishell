/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_noninteractive.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 23:47:41 by yolim             #+#    #+#             */
/*   Updated: 2026/04/16 23:51:14 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_heredoc_noninteractive(t_command *cmd, t_minishell *minishell,
	int *pipe_fd)
{
	char	*raw_line;
	char	*input_line;
	int		result;

	while (1)
	{
		raw_line = get_next_line(STDIN_FILENO);
		if (!raw_line)
		{
			print_heredoc_eof_warning(cmd);
			break ;
		}
		input_line = ft_strtrim(raw_line, "\r\n");
		free(raw_line);
		if (!input_line)
			return ((void)(minishell->last_exit_status = SHELL_FAILURE,
				close_heredoc_pipe_fail(cmd, pipe_fd)));
		result = process_heredoc_line(&input_line, cmd, minishell, pipe_fd[1]);
		if (result == HEREDOC_HIT_DELIMITER)
			break ;
		if (result == HEREDOC_ERROR)
			return ((void)(close_heredoc_pipe_fail(cmd, pipe_fd)));
	}
	close(pipe_fd[1]);
	cmd->heredoc_fd = pipe_fd[0];
}

void	print_heredoc_eof_warning(t_command *cmd)
{
	ft_putstr_fd("bash: warning: here-document delimited by "
		"end-of-file (wanted `", 2);
	ft_putstr_fd(cmd->heredoc_delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

void	close_heredoc_pipe_fail(t_command *cmd, int *pipe_fd)
{
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	cmd->heredoc_fd = -1;
}
