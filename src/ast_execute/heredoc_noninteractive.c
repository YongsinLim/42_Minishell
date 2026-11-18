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

void	process_heredoc_noninteractive(t_redir *redir, t_minishell *minishell,
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
			print_heredoc_eof_warning(redir);
			break ;
		}
		input_line = ft_strtrim(raw_line, "\r\n");
		free(raw_line);
		if (!input_line)
			return (close_heredoc_pipe_fail(minishell, redir, pipe_fd));
		result = process_heredoc_line(&input_line, redir, minishell,
				pipe_fd[1]);
		if (result == HEREDOC_HIT_DELIMITER)
			break ;
		if (result == HEREDOC_ERROR)
			return (close_heredoc_pipe_fail(minishell, redir, pipe_fd));
	}
	close(pipe_fd[1]);
	redir->heredoc_fd = pipe_fd[0];
}

void	print_heredoc_eof_warning(t_redir *redir)
{
	ft_putstr_fd("bash: warning: here-document delimited by "
		"end-of-file (wanted `", 2);
	ft_putstr_fd(redir->file, 2);
	ft_putstr_fd("')\n", 2);
}

void	close_heredoc_pipe_fail(t_minishell *minishell, t_redir *redir,
	int *pipe_fd)
{
	minishell->last_exit_status = SHELL_FAILURE;
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	redir->heredoc_fd = -1;
}

int	open_heredoc_tmp(int *fds)
{
	static int	id;
	char		*num;
	char		*path;

	num = ft_itoa(id++);
	if (!num)
		return (SHELL_FAILURE);
	path = ft_strjoin("/tmp/.minishell_hd_", num);
	free(num);
	if (!path)
		return (SHELL_FAILURE);
	fds[1] = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	fds[0] = open(path, O_RDONLY);
	unlink(path);
	free(path);
	if (fds[1] == -1 || fds[0] == -1)
	{
		if (fds[1] != -1)
			close(fds[1]);
		if (fds[0] != -1)
			close(fds[0]);
		return (SHELL_FAILURE);
	}
	return (SHELL_SUCCESS);
}
