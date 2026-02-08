/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:37:54 by yolim             #+#    #+#             */
/*   Updated: 2026/02/08 13:20:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_input(t_command *cmd)
{
	int	infile_fd;

	if (cmd->heredoc_fd != -1)
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

void	redirect_output(t_command *cmd)
{
	int	outfile_fd;

	if (cmd->output_file)
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

void	execute_pipe_left(t_ast_node *ast, char **envp, int *pipe_fd)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exit(execute_ast(ast->left, envp));
}

void	execute_pipe_right(t_ast_node *ast, char **envp, int *pipe_fd)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exit(execute_ast(ast->right, envp));
}
