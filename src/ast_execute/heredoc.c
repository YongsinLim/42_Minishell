/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:41:50 by yolim             #+#    #+#             */
/*   Updated: 2026/03/25 16:10:08 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredocs(t_ast_node *ast, t_minishell *minishell)
{
	if (!ast)
		return ;
	if (ast->type == NODE_PIPE || ast->type == NODE_AND || ast->type == NODE_OR)
	{
		heredocs(ast->left, minishell);
		heredocs(ast->right, minishell);
	}
	else if (ast->type == NODE_SUBSHELL)
	{
		heredocs(ast->left, minishell);
		if (ast->command && ast->command->heredoc_delimiter != NULL)
			process_heredoc(ast->command, minishell);
	}
	else if (ast->type == NODE_COMMAND)
	{
		if (ast->command && ast->command->heredoc_delimiter != NULL)
			process_heredoc(ast->command, minishell);
	}
}

void	process_heredoc(t_command *cmd, t_minishell *minishell)
{
	int		pipe_fd[2];
	char	*input_line;
	char	*limiter;

	if (pipe(pipe_fd) == -1)
		error_exit("Heredoc pipe failed");
	limiter = ft_strdup(cmd->heredoc_delimiter);
	if (!limiter)
		error_exit("Heredoc limiter malloc failed");
	while (1)
	{
		input_line = readline("heredoc> ");
		if (!input_line)
		{
			free(limiter);
			break ;
		}
		input_line = verify_expand_heredoc(cmd, minishell, input_line, limiter);
		if (!input_line)
		{
			free(limiter);
			break ;
		}
		ft_putstr_fd(input_line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(input_line);
	}
	close(pipe_fd[1]);
	cmd->heredoc_fd = pipe_fd[0];
}

char	*verify_expand_heredoc(t_command *cmd, t_minishell *minishell,
		char *line, char *limiter)
{
	char	*expanded_result;

	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
		&& ft_strlen(line) == ft_strlen(limiter))
	{
		free(line);
		return (NULL);
	}
	if (cmd->heredoc_is_quoted == 0)
	{
		expanded_result = expand_variable(line, minishell);
		free(line);
		line = expanded_result;
	}
	if (!line)
	{
		perror("Heredoc expansion failed");
		return (NULL);
	}
	return (line);
}
