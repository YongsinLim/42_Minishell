/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:37:54 by yolim             #+#    #+#             */
/*   Updated: 2026/04/17 00:23:49 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	apply_single_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd != -1)
	{
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
		return (SHELL_SUCCESS);
	}
	if (redir->type == TOKEN_REDIRECT_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_APPEND)
		fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (redirect_open_error(redir->file));
	if (redir->type == TOKEN_REDIRECT_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (SHELL_SUCCESS);
}

int	apply_redirections(t_command *cmd)
{
	t_redir	*current;

	current = cmd->redirs;
	while (current)
	{
		if (apply_single_redir(current) != SHELL_SUCCESS)
			return (SHELL_FAILURE);
		current = current->next;
	}
	return (SHELL_SUCCESS);
}

int	redirect_open_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	return (SHELL_FAILURE);
}

/*
pipe(pipe_fd[2])
pipe_fd[0]: The read end of the pipe.
pipe_fd[1]: The write end of the pipe.
Return Value: Returns 0 on success and -1 on error.
*/
void	execute_pipe_left(t_ast_node *ast, t_minishell *minishell, int *pipe_fd)
{
	int	status;

	init_signals_child();
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	status = execute_ast(ast->left, minishell);
	cleanup_and_exit(minishell, status);
}

void	execute_pipe_right(t_ast_node *ast, t_minishell *minishell,
	int *pipe_fd)
{
	int	status;

	init_signals_child();
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	status = execute_ast(ast->right, minishell);
	cleanup_and_exit(minishell, status);
}
