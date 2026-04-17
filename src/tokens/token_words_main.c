/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 11:21:28 by yolim             #+#    #+#             */
/*   Updated: 2026/04/17 11:25:29 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Delimiter is never expanded because:
It's a pattern to match against input lines
Must be literally preserved as the user typed it
Otherwise the heredoc can't find where the content ends
 */
int	handle_word(char *line, int *i, t_token **tokens, t_minishell *minishell)
{
	t_token_to_word	word;
	char			*full_word;

	word.minishell = minishell;
	word.is_quoted_string = FALSE;
	word.is_contain_wildcard = FALSE;
	word.is_disabled_expand = previous_token_is_heredoc(*tokens);
	full_word = expand_tilde_prefix(line, i, minishell,
			word.is_disabled_expand);
	while (line[*i] && !is_separator(line[*i]))
	{
		full_word = get_segment_check_wildcard(line, i, &word, full_word);
		if (!full_word)
			return (FALSE);
	}
	create_and_add_word_token(full_word, &word, tokens);
	free(full_word);
	(*i)--;
	return (TRUE);
}

int	previous_token_is_heredoc(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (FALSE);
	current = tokens;
	while (current->next)
		current = current->next;
	return (current->type == TOKEN_HEREDOC);
}

/*
Checking ~ condition :
~ | cat     # Followed by separator (space)
~/folder    # Followed by /
~           # Followed by nothing (\0)
 */
char	*expand_tilde_prefix(char *line, int *i, t_minishell *minishell,
	int disable_expand)
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

char	*get_segment_check_wildcard(char *line, int *i, t_token_to_word *word,
	char *full_word)
{
	char	*segment;

	while (!word->is_disabled_expand && line[*i] == '$'
		&& (line[*i + 1] == '"' || line[*i + 1] == '\''))
		(*i)++;
	if (line[*i] == '"' || line[*i] == '\'')
	{
		word->is_quoted_string = TRUE;
		segment = handle_quoted_string(line, i, word->minishell,
				word->is_disabled_expand);
	}
	else
	{
		segment = get_unquoted_segment(line, i, word->minishell,
				word->is_disabled_expand);
		if (!word->is_disabled_expand && segment && (ft_strchr(segment, '*')
				|| ft_strchr(segment, '?')))
			word->is_contain_wildcard = TRUE;
	}
	return (strjoin_free(full_word, segment));
}
