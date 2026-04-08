/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 21:14:54 by yolim             #+#    #+#             */
/*   Updated: 2026/04/08 21:23:25 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*read_with_unclosed_quotes(char *input, int is_interactive)
{
	char	*next_line;
	char	*trimmed_next;
	char	*combined;

	while (input && has_unclosed_quotes(input))
	{
		if (is_interactive)
			next_line = readline("Minishell > ");
		else
			next_line = get_next_line(STDIN_FILENO);
		if (!next_line)
			break ;
		trimmed_next = ft_strtrim(next_line, "\r\n");
		combined = strjoin_with_nextline(input, trimmed_next);
		free(next_line);
		free(trimmed_next);
		if (!combined)
			break ;
		free(input);
		input = combined;
	}
	return (input);
}

// in_single_quote = !in_single_quote : (0→1 or 1→0)
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

char	*strjoin_with_nextline(char *s1, char *s2)
{
	char	*s1_with_newline;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	s1_with_newline = ft_strjoin(s1, "\n");
	if (!s1_with_newline)
		return (NULL);
	result = ft_strjoin(s1_with_newline, s2);
	free(s1_with_newline);
	return (result);
}
