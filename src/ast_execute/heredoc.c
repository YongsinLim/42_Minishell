/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 16:41:50 by yolim             #+#    #+#             */
/*   Updated: 2026/04/08 14:28:33 by yolim            ###   ########.fr       */
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

char	*read_heredoc_line_simple(void)
{
	char	*line;
	char	buffer[1];
	int		i;
	int		ret;

	line = malloc(1000);
	if (!line)
		return (NULL);
	i = 0;
	write(1, "heredoc> ", 9);
	
	while (i < 999)
	{
		ret = read(0, buffer, 1);
		if (ret == -1)
		{
			if (errno == EINTR) // Interrupted by signal (Ctrl+C)
			{
				if (g_heredoc_interrupted)
				{
					free(line);
					return (NULL);
				}
			}
			// Other error, continue or break depending on needs
			break ;
		}
		if (ret == 0) // EOF (Ctrl+D)
		{
			if (i == 0)
			{
				write(1, "\n", 1); // Add newline before exiting heredoc
				free(line);
				return (NULL);
			}
			break ;
		}
		if (buffer[0] == '\n')
			break ;
		line[i++] = buffer[0];
	}
	line[i] = '\0';
	return (line);
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	g_heredoc_interrupted = 1;
}

void	setup_heredoc_signals(void)
{
	struct sigaction sa;
	
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // No SA_RESTART - ensure read() doesn't restart after signal
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_child_process(t_command *cmd, t_minishell *minishell, int write_fd)
{
	char	*input_line;
	char	*expanded_result;

	setup_heredoc_signals();
	
	while (1)
	{
		if (g_heredoc_interrupted)
		{
			close(write_fd);
			exit(130);
		}
		input_line = read_heredoc_line_simple();
		if (!input_line) // EOF, error, or interrupted
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
		
		ft_putstr_fd(input_line, write_fd);
		ft_putstr_fd("\n", write_fd);
		free(input_line);
	}
	close(write_fd);
}

void	process_heredoc_noninteractive(t_command *cmd, t_minishell *minishell, int *pipe_fd)
{
	char	*input_line;
	char	*line;
	char	*expanded_result;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			input_line = ft_strtrim(line, "\r\n");
			free(line);
		}
		else
			input_line = NULL;
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

void	process_heredoc(t_command *cmd, t_minishell *minishell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	int		interactive;

	if (pipe(pipe_fd) == -1)
		error_exit("Heredoc pipe failed");
	
	interactive = isatty(STDIN_FILENO);
	
	if (interactive)
	{
		pid = fork();
		if (pid == -1)
			error_exit("Heredoc fork failed");
			
		if (pid == 0)
		{
			close(pipe_fd[0]);
			heredoc_child_process(cmd, minishell, pipe_fd[1]);
			exit(0);
		}
		
		close(pipe_fd[1]);
		init_signals_execution();
		if (waitpid(pid, &status, 0) == -1)
		{
			close(pipe_fd[0]);
			cmd->heredoc_fd = -1;
			init_signals_prompt();
			return ;
		}
		
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipe_fd[0]);
			cmd->heredoc_fd = -1;
			minishell->last_exit_status = 130;
		}
		else
		{
			cmd->heredoc_fd = pipe_fd[0];
		}
		init_signals_prompt();
	}
	else
	{
		process_heredoc_noninteractive(cmd, minishell, pipe_fd);
	}
}