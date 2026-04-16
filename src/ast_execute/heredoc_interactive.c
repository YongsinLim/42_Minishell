/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 23:43:37 by yolim             #+#    #+#             */
/*   Updated: 2026/04/17 00:14:29 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_heredoc_interactive(t_command *cmd, t_minishell *minishell,
	int *pipe_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Heredoc fork failed");
	if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_child_process(cmd, minishell, pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	init_signals_execution();
	if (waitpid(pid, &status, 0) == -1)
	{
		close(pipe_fd[0]);
		cmd->heredoc_fd = -1;
		minishell->last_exit_status = SHELL_FAILURE;
		init_signals_prompt();
		return ;
	}
	set_heredoc_exit_status(cmd, minishell, status, pipe_fd[0]);
	init_signals_prompt();
}

void	heredoc_child_process(t_command *cmd, t_minishell *minishell,
	int write_fd)
{
	char	*input_line;
	int		result;

	setup_heredoc_signals();
	while (1)
	{
		errno = 0;
		input_line = read_heredoc_line();
		if (g_signal == SIGINT)
			close_fd_exit(write_fd, 130);
		if (!input_line)
		{
			if (g_signal != SIGINT && errno != EIO)
				print_heredoc_eof_warning(cmd);
			if (errno == EIO)
				close_fd_exit(write_fd, SHELL_FAILURE);
			break ;
		}
		result = process_heredoc_line(&input_line, cmd, minishell, write_fd);
		if (result == HEREDOC_HIT_DELIMITER)
			break ;
		if (result == HEREDOC_ERROR)
			close_fd_exit(write_fd, SHELL_FAILURE);
	}
	close(write_fd);
}

// Ctrl+C interrupts read() and sets g_signal = SIGINT.
// No SA_RESTART - ensure read() doesn't restart after signal
void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/*
read() return 0 : EOF (Ctrl+D)

i < 1024 - 1 : reserve 1 byte for '\0'
*/
char	*read_heredoc_line(void)
{
	char	*line;
	int		i;
	char	buffer[1];
	int		state;

	line = malloc(1024);
	if (!line)
		return (NULL);
	i = 0;
	write(1, "heredoc> ", 9);
	while (i < 1024 - 1)
	{
		state = handle_read_result(read(0, buffer, 1), i);
		if (state == -1)
			return (free(line), NULL);
		if (state == -2)
			return (free(line), NULL);
		if (state == 1)
			break ;
		if (buffer[0] == '\n')
			break ;
		line[i++] = buffer[0];
	}
	line[i] = '\0';
	return (line);
}

void	set_heredoc_exit_status(t_command *cmd, t_minishell *minishell,
	int status, int read_fd)
{
	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
		|| (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
	{
		close(read_fd);
		cmd->heredoc_fd = -1;
		minishell->last_exit_status = 130;
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(read_fd);
		cmd->heredoc_fd = -1;
		minishell->last_exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		close(read_fd);
		cmd->heredoc_fd = -1;
		minishell->last_exit_status = 128 + WTERMSIG(status);
	}
	else
		cmd->heredoc_fd = read_fd;
}
