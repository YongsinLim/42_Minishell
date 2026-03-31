/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:37:54 by yolim             #+#    #+#             */
/*   Updated: 2026/03/31 22:47:29 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_input(t_command *cmd)
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
				error_exit("Input file error"); //might need to change to specify filename
			dup2(infile_fd, STDIN_FILENO);
			close(infile_fd);
		}
		current = current->next;
	}
}

void	redirect_output(t_command *cmd)
{
	t_redir	*current;
	int		outfile_fd;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_APPEND)
		{
			if (current->type == TOKEN_APPEND)
				outfile_fd = open(current->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			else
				outfile_fd = open(current->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			
			if (outfile_fd < 0)
				error_exit("Output file error");
			dup2(outfile_fd, STDOUT_FILENO);
			close(outfile_fd);
		}
		current = current->next;
	}
}

void	execute_pipe_left(t_ast_node *ast, t_minishell *minishell, int *pipe_fd)
{
	int	status;

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

	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	status = execute_ast(ast->right, minishell);
	cleanup_and_exit(minishell, status);
}
