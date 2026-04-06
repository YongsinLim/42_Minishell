/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:26:41 by yolim             #+#    #+#             */
/*   Updated: 2026/04/06 12:31:54 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_unclosed_quotes(char *str)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

char	*ft_strjoin_with_newline(char *s1, char *s2)
{
	char	*with_newline;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	with_newline = ft_strjoin(s1, "\n");
	if (!with_newline)
		return (NULL);
	result = ft_strjoin(with_newline, s2);
	free(with_newline);
	return (result);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	char		*line;
	int			interactive;
	char		*raw_line;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);

	// init_signals_prompt();
	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
		{
			raw_line = readline("Minishell > ");
			minishell.input = (raw_line) ? ft_strtrim(raw_line, "\r\n") : NULL;
			free(raw_line);
		}
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line)
			{
				minishell.input = ft_strtrim(line, "\r\n");
				free(line);
				// Check for unclosed quotes and continue reading if needed
				while (has_unclosed_quotes(minishell.input))
				{
					char *next_line = get_next_line(STDIN_FILENO);
					if (next_line)
					{
						char *trimmed_next = ft_strtrim(next_line, "\r\n");
						char *combined = ft_strjoin_with_newline(minishell.input,
							trimmed_next);
						free(minishell.input);
						free(trimmed_next);
						free(next_line);
						minishell.input = combined;
					}
					else
						break ;
				}
			}
			else
				minishell.input = NULL;
		}
		if (!minishell.input)
		{
			if (interactive)
				ft_putstr_fd("exit\n", 2); // on EOF or Ctrl + D
			break ;
		}
		if (!is_all_whitespace(minishell.input))
		{
			if (interactive)
				add_history(minishell.input);
			add_to_history(minishell.input, &minishell.history_list);
		}
		minishell.tokens = tokenize(minishell.input, &minishell);
		if (!minishell.tokens)
		{
			free(minishell.input);
			minishell.input = NULL;
			if (!interactive && minishell.last_exit_status == SYNTAX_ERROR)
				break ;
			continue ;
		}
		execution(&minishell);
		if (!interactive && minishell.last_exit_status == SYNTAX_ERROR)
			break ;
	}
	cleanup_and_exit(&minishell, minishell.last_exit_status);
}

void	increment_shlvl(t_minishell *minishell)
{
	char	*shlvl_str;
	int		shlvl_num;
	char	*new_shlvl_str;

	shlvl_str = get_var_value("SHLVL", minishell);
	if (shlvl_str[0] == '\0')
	{
		update_env("SHLVL", "1", minishell);
	}
	else
	{
		shlvl_num = ft_atoi(shlvl_str);
		shlvl_num++;
		new_shlvl_str = ft_itoa(shlvl_num);
		if (new_shlvl_str)
		{
			update_env("SHLVL", new_shlvl_str, minishell);
			free(new_shlvl_str);
		}
	}
	free(shlvl_str);
}

void	init_minishell(t_minishell *minishell, char **envp)
{
	minishell->history_list = NULL;
	minishell->env_list = init_env(envp);
	minishell->last_exit_status = 0;
	minishell->input = NULL;
	minishell->tokens = NULL;
	minishell->ast = NULL;
	update_env("OLDPWD", NULL, minishell);
	increment_shlvl(minishell);
}

int	is_all_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	execution(t_minishell *minishell)
{
	minishell->ast = parse(&minishell->tokens);
	if (minishell->ast != NULL)
	{
		heredocs(minishell->ast, minishell);
		minishell->last_exit_status = execute_ast(minishell->ast, minishell);
	}
	else
		minishell->last_exit_status = SYNTAX_ERROR;
	free(minishell->input);
	minishell->input = NULL;
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
}

void	cleanup_and_exit(t_minishell *minishell, int exit_status)
{
	free_env_list(minishell->env_list);
	free_history(&minishell->history_list);
	free(minishell->input);
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
	rl_clear_history(); // TODO : check what function do
	exit(exit_status);
}

/*
add_history :
Purpose : adds the user's input to GNU Readline's internal history list.
Functionality : allows user to press UP and DOWN arrow keys to cycle through
previous commands.
*/