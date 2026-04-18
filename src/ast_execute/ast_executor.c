/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:04:14 by yolim             #+#    #+#             */
/*   Updated: 2026/04/18 13:42:30 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_ast(t_ast_node *ast, t_minishell *minishell)
{
	int	status;

	if (!ast)
		return (SHELL_SUCCESS);
	if (ast->type == NODE_COMMAND)
		return (execute_simple_command(ast, minishell));
	if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, minishell));
	if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		status = execute_ast(ast->left, minishell);
		if ((ast->type == NODE_AND && status == SHELL_SUCCESS)
			|| (ast->type == NODE_OR && status != SHELL_SUCCESS))
			return (execute_ast(ast->right, minishell));
		return (status);
	}
	if (ast->type == NODE_SUBSHELL)
		return (execute_subshell(ast, minishell));
	return (SHELL_FAILURE);
}

int	execute_pipe(t_ast_node *ast, t_minishell *minishell)
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipe_fd) == -1)
		error_exit("Pipe failed");
	pid_left = fork();
	if (pid_left == -1)
		error_exit("Fork Error for pipe_left");
	if (pid_left == 0)
		execute_pipe_left(ast, minishell, pipe_fd);
	pid_right = fork();
	if (pid_right == -1)
		error_exit("Fork Error for pipe_right");
	if (pid_right == 0)
		execute_pipe_right(ast, minishell, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_pipeline_status(pid_right));
}

/*
check : waitpid(-1, &status, 0) != -1
`waitpid` returns -1 when there are no more child processes to wait for.
to avoids zombie side effects that can look like shell "stuck" behavior
in longer runs.
 */
int	wait_pipeline_status(pid_t pid_right)
{
	int		right_status;
	int		status;

	init_signals_execution();
	if (waitpid(pid_right, &right_status, 0) == -1)
	{
		init_signals_prompt();
		return (SHELL_FAILURE);
	}
	while (waitpid(-1, &status, 0) != -1)
		;
	init_signals_prompt();
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	if (WIFSIGNALED(right_status))
	{
		if (WTERMSIG(right_status) == SIGINT)
			write(1, "\n", 1);
		return (128 + WTERMSIG(right_status));
	}
	return (SHELL_FAILURE);
}

int	execute_subshell(t_ast_node *ast, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Fork Error for subshell");
	if (pid == 0)
	{
		init_signals_child();
		if (ast->command)
		{
			if (redirect_input(ast->command) != SHELL_SUCCESS
				|| redirect_output(ast->command) != SHELL_SUCCESS)
				cleanup_and_exit(minishell, SHELL_FAILURE);
		}
		status = execute_ast(ast->left, minishell);
		cleanup_and_exit(minishell, status);
	}
	if (ast->command && ast->command->heredoc_fd != -1)
	{
		close(ast->command->heredoc_fd);
		ast->command->heredoc_fd = -1;
	}
	init_signals_execution();
	status = wait_for_children(pid);
	init_signals_prompt();
	return (status);
}
