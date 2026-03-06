/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/06 16:42:18 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*init_word(char *line, int *i, t_minishell *minishell)
{
	char	*home;

	if (line[*i] == '~' && (is_separator(line[*i + 1]) || line[*i + 1] == '/'
			|| line[*i + 1] == '\0'))
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

char	*handle_quoted_string(char *line, int *i, t_minishell *minishell)
{
	char	quote_char;
	int		start_idx;
	char	*quoted_str;
	char	*expanded_str;

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
	if (quote_char == '"')
		expanded_str = expand_variable(quoted_str, minishell);
	else
		expanded_str = ft_strdup(quoted_str);
	free(quoted_str);
	return (expanded_str);
}

char	*get_unquoted_segment(char *line, int *i, t_minishell *minishell)
{
	int		start_idx;
	char	*word;
	char	*expanded;

	start_idx = *i;
	while (line[*i] && !is_separator(line[*i]) && line [*i] != '"'
		&& line[*i] != '\'')
		(*i)++;
	word = ft_substr(line, start_idx, *i - start_idx);
	expanded = expand_variable(word, minishell);
	free(word);
	return (expanded);
}

void	handle_word(char *line, int *i, t_token **tokens,
		t_minishell *minishell)
{
	int		has_quotes;
	char	*full_word;
	char	*segment;
	char	*temp;

	has_quotes = FALSE;
	full_word = init_word(line, i, minishell);
	while (line[*i] && !is_separator(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			has_quotes = TRUE;
			segment = handle_quoted_string(line, i, minishell);
		}
		else
			segment = get_unquoted_segment(line, i, minishell);
		if (!segment)
		{
			free(full_word);
			return ;
		}
		temp = ft_strjoin(full_word, segment);
		free(full_word);
		free(segment);
		full_word = temp;
	}
	if (full_word && (full_word[0] != '\0' || has_quotes))
		add_token(tokens, new_token(full_word, TOKEN_WORD, has_quotes));
	free(full_word);
	(*i)--;
}
