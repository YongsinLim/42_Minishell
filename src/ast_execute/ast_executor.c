/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:04:14 by yolim             #+#    #+#             */
/*   Updated: 2026/02/09 13:19:37 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_ast(t_ast_node *ast, char **envp)
{
	int	status;

	if (!ast)
		return (SHELL_SUCCESS);
	if (ast->type == NODE_COMMAND)
		return (execute_simple_command(ast, envp));
	if (ast->type == NODE_PIPE)
		return (exec_pipe(ast, envp));
	if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		status = execute_ast(ast->left, envp);
		if ((ast->type == NODE_AND && status == SHELL_SUCCESS)
			|| (ast->type == NODE_OR && status != SHELL_SUCCESS))
			return (execute_ast(ast->right, envp));
		return (status);
	}
	if (ast->type == NODE_SUBSHELL)
		return (exec_subshell(ast, envp));
	return (SHELL_FAILURE);
}

/*
fork() :
-1: The creation of the child process was unsuccessful due to an error
0: The code is executing within the child process.
Positive value (the child's PID): The code is executing within the parent process
*/

int	execute_simple_command(t_ast_node *ast, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Fork Error");
	if (pid == 0)
	{
		redirect_input(ast->command);
		redirect_output(ast->command);
		execute(ast->command->argv, envp);
		exit(CMD_NOT_FOUND);
	}
	if (ast->command->heredoc_fd != -1)
	{
		close(ast->command->heredoc_fd);
		ast->command->heredoc_fd = -1;
	}
	status = wait_for_children(pid);
	return (status);
}

int	exec_pipe(t_ast_node *ast, char **envp)
{
	pid_t	pid_left;
	pid_t	pid_right;
	int		pipe_fd[2];
	int		status;

	if (pipe(pipe_fd) == -1)
		error_exit("Pipe failed");
	pid_left = fork();
	if (pid_left == -1)
		error_exit("Fork Error for pipe_left");
	if (pid_left == 0)
		execute_pipe_left(ast, envp, pipe_fd);
	pid_right = fork();
	if (pid_right == -1)
		error_exit("Fork Error for pipe_right");
	if (pid_right == 0)
		execute_pipe_right(ast, envp, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait_for_children(pid_left);
	status = wait_for_children(pid_right);
	return (status);
}

int	exec_subshell(t_ast_node *ast, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Fork Error for subshell");
	if (pid == 0)
	{
		status = execute_ast(ast->left, envp);
		exit (status);
	}
	if (ast->command && ast->command->heredoc_fd != -1)
	{
		close(ast->command->heredoc_fd);
		ast->command->heredoc_fd = -1;
	}
	status = wait_for_children(pid);
	return (status);
}
