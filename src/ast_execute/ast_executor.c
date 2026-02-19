/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:04:14 by yolim             #+#    #+#             */
/*   Updated: 2026/02/19 23:18:45 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int execute_ast(t_ast_node *ast, t_env **env_list)
{
    int status;

    if (!ast)
        return (SHELL_SUCCESS);
    if (ast->type == NODE_COMMAND)
        return (execute_simple_command(ast, env_list));
    if (ast->type == NODE_PIPE)
        return (exec_pipe(ast, env_list));
    if (ast->type == NODE_AND || ast->type == NODE_OR)
    {
        status = execute_ast(ast->left, env_list);
        if ((ast->type == NODE_AND && status == SHELL_SUCCESS)
            || (ast->type == NODE_OR && status != SHELL_SUCCESS))
            return (execute_ast(ast->right, env_list));
        return (status);
    }
    if (ast->type == NODE_SUBSHELL)
        return (exec_subshell(ast, env_list));
    return (SHELL_FAILURE);
}

/*
fork() :
-1: The creation of the child process was unsuccessful due to an error
0: The code is executing within the child process.
Positive value (the child's PID): The code is executing within the parent process
*/

int execute_simple_command(t_ast_node *ast, t_env **env_list)
{
    pid_t pid;
    int status;

    if (!ast || !ast->command || !ast->command->argv || !ast->command->argv[0])
        return (SHELL_SUCCESS);
    if (is_builtin(ast->command->argv[0]))
        return (run_builtin_parent(ast, env_list));
    pid = fork();
    if (pid == -1)
        error_exit("Fork Error");
    if (pid == 0)
    {
        redirect_input(ast->command);
        redirect_output(ast->command);
        execute(ast->command->argv, env_list); // Pass env_list here
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

int	exec_pipe(t_ast_node *ast, t_env **env_list)
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
		execute_pipe_left(ast, env_list, pipe_fd);
	pid_right = fork();
	if (pid_right == -1)
		error_exit("Fork Error for pipe_right");
	if (pid_right == 0)
		execute_pipe_right(ast, env_list, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait_for_children(pid_left);
	status = wait_for_children(pid_right);
	return (status);
}

int	exec_subshell(t_ast_node *ast, t_env **env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Fork Error for subshell");
	if (pid == 0)
	{
		status = execute_ast(ast->left, env_list);
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

int run_builtin_parent(t_ast_node *ast, t_env **env_list)
{
    int saved_stdin;
    int saved_stdout;
    int status;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    redirect_input(ast->command);
    redirect_output(ast->command);
    status = exec_builtin(ast->command->argv, env_list);
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