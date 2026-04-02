/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/02 15:52:58 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*init_word(char *line, int *i, t_minishell *minishell, int disable_expand)
{
	char	*home;

	if (!disable_expand && line[*i] == '~' && (is_separator(line[*i + 1])
		|| line[*i + 1] == '/' || line[*i + 1] == '\0'))
	{
		home = get_var_value("HOME", minishell);
		(*i)++;
		if (!home || home[0] == '\0')
		{
			free(home);
			return (ft_strdup("~"));
		}
		return (home);
	}
	return (ft_strdup(""));
}

// Check if char is a separator
int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

char	*handle_quoted_string(char *line, int *i, t_minishell *minishell,
		int disable_expand)
{
	char	quote_char;
	int		start_idx;
	char	*quoted_str;
	char	*expanded_str;
	int		k = 0;

	quote_char = line[*i];
	(*i)++;
	start_idx = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (minishell->last_exit_status = 2, NULL);
	}
	quoted_str = ft_substr(line, start_idx, *i - start_idx);
	(*i)++;
	if (quote_char == '"' && !disable_expand)
		expanded_str = expand_variable(quoted_str, minishell);
	else
		expanded_str = ft_strdup(quoted_str);
	free(quoted_str);
	k = 0;
	while (expanded_str && expanded_str[k])
	{
		if (expanded_str[k] == ' ')
			expanded_str[k] = '\x1A';
		else if (expanded_str[k] == '\t')
			expanded_str[k] = '\x1B';
		else if (expanded_str[k] == '\n')
			expanded_str[k] = '\x1C';
		k++;
	}
	return (expanded_str);
}

char	*get_unquoted_segment(char *line, int *i, t_minishell *minishell,
		int disable_expand)
{
	int		start_idx;
	char	*word;
	char	*expanded;

	start_idx = *i;
	while (line[*i] && !is_separator(line[*i]) && line [*i] != '"'
		&& line[*i] != '\'')
		(*i)++;
	word = ft_substr(line, start_idx, *i - start_idx);
	if (disable_expand)
		expanded = ft_strdup(word);
	else
		expanded = expand_variable(word, minishell);
	free(word);
	return (expanded);
}

char	*strjoin_free(char *full_word, char *segment)
{
	char	*new;

	if (!segment)
	{
		free(full_word);
		return (NULL);
	}
	new = ft_strjoin(full_word, segment);
	free(full_word);
	free(segment);
	return (new);
}
