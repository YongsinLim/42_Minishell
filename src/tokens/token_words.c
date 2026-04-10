/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/10 15:59:11 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// expand $ sign only in double-quoted
char	*handle_quoted_string(char *line, int *i, t_minishell *minishell,
		int disable_expand)
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
		minishell->last_exit_status = SYNTAX_ERROR;
		return (NULL);
	}
	quoted_str = ft_substr(line, start_idx, *i - start_idx);
	(*i)++;
	if (quote_char == '"' && !disable_expand)
		expanded_str = expand_variable(quoted_str, minishell);
	else
		expanded_str = ft_strdup(quoted_str);
	free(quoted_str);
	escape_whitespace(expanded_str);
	return (expanded_str);
}

void	escape_whitespace(char *str)
{
	int	k;

	k = 0;
	while (str && str[k])
	{
		if (str[k] == ' ')
			str[k] = '\x1A';
		else if (str[k] == '\t')
			str[k] = '\x1B';
		else if (str[k] == '\n')
			str[k] = '\x1C';
		k++;
	}
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

void	create_and_add_word_token(char *full_word, t_token_to_word *word,
		t_token **tokens)
{
	t_token	*new_node;

	if (full_word && (full_word[0] != '\0' || word->is_quoted_string))
	{
		new_node = new_token(full_word, TOKEN_WORD, word->is_quoted_string);
		new_node->has_wildcard = word->is_contain_wildcard;
		add_token(tokens, new_node);
	}
}
