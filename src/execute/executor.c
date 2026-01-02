/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:02:49 by yolim             #+#    #+#             */
/*   Updated: 2026/01/02 18:32:15 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_pipeline(t_command *pipeline, char *envp[])
{
	int		previous_pipe_read_end;
	int		pipe_fd[2];
	pid_t	fork_pid_result;

	if (!pipeline)
		return (0);
	previous_pipe_read_end = -1;
	while (pipeline != NULL)
	{
		if (pipeline->next != NULL)
			if (pipe(pipe_fd) == -1)
				error_exit("Pipe failed");
		fork_pid_result = fork();
		if (fork_pid_result == -1)
			error_exit("Fork Error");
		if (fork_pid_result == 0)
		{
			redirect_input(previous_pipe_read_end, pipeline);
			redirect_output(pipeline, pipe_fd);
			execute(pipeline->argv, envp);
		}
		parent_pipe_handler(&previous_pipe_read_end, pipeline, pipe_fd);
		pipeline = pipeline->next;
	}
	return (cleanup_and_wait(previous_pipe_read_end, fork_pid_result));
}

void	redirect_input(int prev_pipe_read_end, t_command *cmd)
{
	int	infile_fd;

	if (prev_pipe_read_end != -1)
	{
		dup2(prev_pipe_read_end, STDIN_FILENO);
		close(prev_pipe_read_end);
	}
	else if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	else if (cmd->input_file)
	{
		infile_fd = open(cmd->input_file, O_RDONLY);
		if (infile_fd < 0)
			error_exit("Input file error");
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
}

void	redirect_output(t_command *cmd, int *pipe_fd)
{
	int	outfile_fd;

	if (cmd->next != NULL)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else if (cmd->output_file)
	{
		if (cmd->is_append)
			outfile_fd = open(cmd->output_file, O_CREAT | O_WRONLY
					| O_APPEND, 0644);
		else
			outfile_fd = open(cmd->output_file, O_CREAT | O_WRONLY
					| O_TRUNC, 0644);
		if (outfile_fd < 0)
			error_exit("Output file error");
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
	}
}

void	parent_pipe_handler(int *previous_pipe_read_end, t_command *pipeline,
		int pipe_fd[2])
{
	if (*previous_pipe_read_end != -1)
		close(*previous_pipe_read_end);
	if (pipeline->next != NULL)
	{
		close(pipe_fd[1]);
		*previous_pipe_read_end = pipe_fd[0];
	}
}

int	cleanup_and_wait(int previous_pipe_read_end, pid_t fork_pid_result)
{
	int	status;

	if (previous_pipe_read_end != -1)
		close(previous_pipe_read_end);
	status = 0;
	if (fork_pid_result > 0)
		status = wait_for_children(fork_pid_result);
	return (status);
}
