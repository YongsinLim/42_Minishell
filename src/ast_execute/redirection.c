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

int	redirect_input(t_command *cmd)
{
	t_redir	*current;
	int		infile_fd;

	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	current = cmd->redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN)
		{
			infile_fd = open(current->file, O_RDONLY);
			if (infile_fd < 0)
				return (redirect_open_error(current->file));
			dup2(infile_fd, STDIN_FILENO);
			close(infile_fd);
		}
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

int	redirect_output(t_command *cmd)
{
	t_redir	*current;
	int		outfile_fd;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_APPEND)
		{
			if (current->type == TOKEN_APPEND)
				outfile_fd = open(current->file, O_CREAT | O_WRONLY | O_APPEND,
						0644);
			else
				outfile_fd = open(current->file, O_CREAT | O_WRONLY | O_TRUNC,
						0644);
			if (outfile_fd < 0)
				return (redirect_open_error(current->file));
			dup2(outfile_fd, STDOUT_FILENO);
			close(outfile_fd);
		}
		current = current->next;
	}
	return (SHELL_SUCCESS);
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
