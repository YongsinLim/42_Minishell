/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:04:14 by yolim             #+#    #+#             */
/*   Updated: 2026/03/01 20:04:28 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_ast(t_ast_node *ast, t_minishell *minishell)
{
    int status;

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
*/


int	execute_simple_command(t_ast_node *ast, t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	int		stdin_backup;
	int		stdout_backup;

	if (is_builtin(ast->command->argv[0]))
	{
		stdin_backup = dup(STDIN_FILENO);
		stdout_backup = dup(STDOUT_FILENO);
		redirect_input(ast->command);
		redirect_output(ast->command);
		status = execute_builtin(ast->command->argv, minishell);
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (status);
	}
	pid = fork();
	if (pid == -1)
		error_exit("Fork Error");
	if (pid == 0)
	{
		redirect_input(ast->command);
		redirect_output(ast->command);
		minishell->last_exit_status = 127;
		execute(ast->command->argv, minishell);
		cleanup_and_exit(minishell, minishell->last_exit_status);
	}
	if (ast->command->heredoc_fd != -1)
	{
		close(ast->command->heredoc_fd);
		ast->command->heredoc_fd = -1;
	}
	status = wait_for_children(pid);
	return (status);
}

int	exec_pipe(t_ast_node *ast, t_minishell *minishell)
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
		execute_pipe_left(ast, minishell, pipe_fd);
	pid_right = fork();
	if (pid_right == -1)
		error_exit("Fork Error for pipe_right");
	if (pid_right == 0)
		execute_pipe_right(ast, minishell, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait_for_children(pid_left);
	status = wait_for_children(pid_right);
	return (status);
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
		status = execute_ast(ast->left, minishell);
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

int run_builtin_parent(t_ast_node *ast, t_minishell *minishell)
{
    int saved_stdin;
    int saved_stdout;
    int status;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    redirect_input(ast->command);
    redirect_output(ast->command);
    status = exec_builtin(ast->command->argv, &minishell->env_list);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    if (ast->command->heredoc_fd != -1)
    {
        close(ast->command->heredoc_fd);
        ast->command->heredoc_fd = -1;
    }
    return (status);
}