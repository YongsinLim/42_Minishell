/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:41:50 by yolim             #+#    #+#             */
/*   Updated: 2026/04/06 14:35:00 by yolim            ###   ########.fr       */
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
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error_exit("Heredoc fork failed");
	if (pid == 0)
	{
		// Child process - handles heredoc input
		init_signals_heredoc();
		heredoc_child_process(cmd, minishell);
		exit(0);  // Child exits after heredoc completes
	}
	// Parent waits for heredoc child to finish
	waitpid(pid, &status, 0);
	// Return to prompt signal handling
	init_signals_prompt();
}

void	heredoc_child_process(t_command *cmd, t_minishell *minishell)
{
	int		pipe_fd[2];
	char	*input_line;
	char	*line;
	char	*expanded_result;
	int		interactive;

	if (pipe(pipe_fd) == -1)
		error_exit("Heredoc pipe failed");
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
			input_line = readline("heredoc> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line)
			{
				input_line = ft_strtrim(line, "\r\n");
				free(line);
			}
			else
				input_line = NULL;
		}
		if (!input_line)
			break ;
		if (ft_strncmp(input_line, cmd->heredoc_delimiter,
				ft_strlen(cmd->heredoc_delimiter) + 1) == 0)
		{
			free(input_line);
			break ;
		}
		if (cmd->heredoc_is_quoted == 0)
		{
			expanded_result = expand_variable(input_line, minishell);
			free(input_line);
			input_line = expanded_result;
			if (!input_line)
				break ;
		}
		ft_putstr_fd(input_line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(input_line);
	}
	close(pipe_fd[1]);
	cmd->heredoc_fd = pipe_fd[0];
}
