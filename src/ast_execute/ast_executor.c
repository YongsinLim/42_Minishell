/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:04:14 by yolim             #+#    #+#             */
/*   Updated: 2026/04/12 19:51:34 by yolim            ###   ########.fr       */
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
		return (exec_pipe(ast, minishell));
	if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		status = execute_ast(ast->left, minishell);
		if ((ast->type == NODE_AND && status == SHELL_SUCCESS)
			|| (ast->type == NODE_OR && status != SHELL_SUCCESS))
			return (execute_ast(ast->right, minishell));
		return (status);
	}
	if (ast->type == NODE_SUBSHELL)
		return (exec_subshell(ast, minishell));
	return (SHELL_FAILURE);
}

/*
fork() :
-1: The creation of the child process was unsuccessful due to an error
0: The code is executing within the child process.
Positive value (the child's PID): The code is executing within the parent process

To check edge case : < file or > file (without command to run)
if (!ast->command->argv[0])
		return (redirect_input(ast->command), redirect_output(ast->command),
			SHELL_SUCCESS);
*/

int	execute_simple_command(t_ast_node *ast, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	if (!ast->command->argv[0])
	{
		if (redirect_input(ast->command) != SHELL_SUCCESS
			|| redirect_output(ast->command) != SHELL_SUCCESS)
			return (SHELL_FAILURE);
		return (SHELL_SUCCESS);
	}
	if (is_builtin(ast->command->argv[0]))
	{
		// Only treat 'env' as builtin if it has NO arguments
		if (ft_strncmp(ast->command->argv[0], "env", 4) == 0
			&& ast->command->argv[1] != NULL)
		{
			// env with arguments -> execute as external command
			pid = fork();
			if (pid == 0)
			{
				init_signals_child();
				if (redirect_input(ast->command) != SHELL_SUCCESS
					|| redirect_output(ast->command) != SHELL_SUCCESS)
					cleanup_and_exit(minishell, SHELL_FAILURE);
				execute(ast->command->argv, minishell);
				cleanup_and_exit(minishell, minishell->last_exit_status);
			}
			init_signals_execution();
			status = wait_for_children(pid);
			init_signals_prompt();
			return (status);
		}
		return (handle_builtin_execution(ast, minishell));
	}
	pid = fork();
	if (pid == -1)
		error_exit("Fork Error");
	if (pid == 0)
	{
		init_signals_child();
		if (redirect_input(ast->command) != SHELL_SUCCESS
			|| redirect_output(ast->command) != SHELL_SUCCESS)
			cleanup_and_exit(minishell, SHELL_FAILURE);
		minishell->last_exit_status = 127;
		execute(ast->command->argv, minishell);
		cleanup_and_exit(minishell, minishell->last_exit_status);
	}
	init_signals_execution();
	if (ast->command->heredoc_fd != -1)
	{
		close(ast->command->heredoc_fd);
		ast->command->heredoc_fd = -1;
	}
	status = wait_for_children(pid);
	init_signals_prompt();
	return (status);
}

int	handle_builtin_execution(t_ast_node *ast, t_minishell *minishell)
{
	int	status;
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (redirect_input(ast->command) != SHELL_SUCCESS
		|| redirect_output(ast->command) != SHELL_SUCCESS)
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (SHELL_FAILURE);
	}
	status = execute_builtin(ast->command->argv, minishell);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

int	exec_pipe(t_ast_node *ast, t_minishell *minishell)
{
	pid_t	pid_left;
	pid_t	pid_right;
	int		pipe_fd[2];
	int		status;
	int		right_status;

	if (pipe(pipe_fd) == -1)
		error_exit("Pipe failed");
	pid_left = fork();
	if (pid_left == -1)
		error_exit("Fork Error for pipe_left");
	if (pid_left == 0)
	{
		execute_pipe_left(ast, minishell, pipe_fd);
		init_signals_child();
	}
	pid_right = fork();
	if (pid_right == -1)
		error_exit("Fork Error for pipe_right");
	if (pid_right == 0)
	{
		execute_pipe_right(ast, minishell, pipe_fd);
		init_signals_child();
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	init_signals_execution();
	if (waitpid(pid_right, &right_status, 0) == -1)
	{
		init_signals_prompt();
		return (SHELL_FAILURE);
	}
	while (waitpid(-1, &status, 0) != -1)
		; // avoids zombie side effects that can look like shell "stuck"
			// behavior in longer runs.
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

int	exec_subshell(t_ast_node *ast, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Fork Error for subshell");
	if (pid == 0)
	{
		init_signals_child();
		status = execute_ast(ast->left, minishell);
		exit (status);
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
