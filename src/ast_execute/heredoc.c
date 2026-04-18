/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:41:50 by yolim             #+#    #+#             */
/*   Updated: 2026/04/17 00:15:22 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
exit code 130 : process was terminated via a SIGINT (Interrupt) signal (Ctrl+C)

if ast->type == NODE_SUBSHELL, only execute ast->left,
bcos node_subshell only store at ast->left, ast->right = NULL (no use).
 */
int	heredocs(t_ast_node *ast, t_minishell *minishell)
{
	int	status;

	if (!ast)
		return (SHELL_SUCCESS);
	if (ast->type == NODE_PIPE || ast->type == NODE_AND || ast->type == NODE_OR)
	{
		status = heredocs(ast->left, minishell);
		if (status != SHELL_SUCCESS)
			return (status);
		status = heredocs(ast->right, minishell);
		if (status != SHELL_SUCCESS)
			return (status);
	}
	else if (ast->type == NODE_SUBSHELL)
	{
		status = heredocs(ast->left, minishell);
		if (status != SHELL_SUCCESS)
			return (status);
		return (heredocs_command_node(ast, minishell));
	}
	else if (ast->type == NODE_COMMAND)
		return (heredocs_command_node(ast, minishell));
	return (SHELL_SUCCESS);
}

int	heredocs_command_node(t_ast_node *ast, t_minishell *minishell)
{
	if (ast->command && ast->command->heredoc_delimiter)
	{
		process_heredoc(ast->command, minishell);
		if (ast->command->heredoc_fd == -1)
			return (minishell->last_exit_status);
	}
	return (SHELL_SUCCESS);
}

void	process_heredoc(t_command *cmd, t_minishell *minishell)
{
	int		pipe_fd[2];
	int		interactive;

	if (pipe(pipe_fd) == -1)
		error_exit("Heredoc pipe failed");
	interactive = isatty(STDIN_FILENO);
	if (!interactive)
		return (process_heredoc_noninteractive(cmd, minishell, pipe_fd));
	process_heredoc_interactive(cmd, minishell, pipe_fd);
}

int	process_heredoc_line(char **input_line, t_command *cmd,
	t_minishell *minishell, int write_fd)
{
	char	*expanded_result;

	if (ft_strncmp(*input_line, cmd->heredoc_delimiter,
			ft_strlen(cmd->heredoc_delimiter) + 1) == 0)
	{
		free(*input_line);
		*input_line = NULL;
		return (HEREDOC_HIT_DELIMITER);
	}
	if (cmd->heredoc_is_quoted == 0)
	{
		expanded_result = expand_variable(*input_line, minishell);
		free(*input_line);
		*input_line = expanded_result;
		if (!*input_line)
		{
			ft_putstr_fd("minishell: heredoc: expansion failed\n", 2);
			return (minishell->last_exit_status = SHELL_FAILURE, HEREDOC_ERROR);
		}
	}
	ft_putstr_fd(*input_line, write_fd);
	ft_putstr_fd("\n", write_fd);
	free(*input_line);
	*input_line = NULL;
	return (HEREDOC_CONTINUE);
}

// EINTR : read was interrupted by signal (Ctrl+C)
int	handle_read_result(ssize_t read_bytes, int i)
{
	if (read_bytes > 0)
		return (0);
	if (read_bytes == -1)
	{
		if (errno == EINTR && g_signal == SIGINT)
			return (-1);
		return (-2);
	}
	if (read_bytes == 0)
	{
		if (i == 0)
		{
			write(1, "\n", 1);
			return (-1);
		}
	}
	return (1);
}
