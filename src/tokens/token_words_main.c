/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:24:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/06 12:46:47 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	last_token_is_heredoc(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (FALSE);
	current = tokens;
	while (current->next)
		current = current->next;
	return (current->type == TOKEN_HEREDOC);
}

void	create_and_add_token(char *full_word, int *flags, t_token **tokens)
{
	t_token	*new_node;

	if (full_word && (full_word[0] != '\0' || flags[0]))
	{
		new_node = new_token(full_word, TOKEN_WORD, flags[0]);
		new_node->has_wildcard = flags[1];
		add_token(tokens, new_node);
	}
}

// flags[0] = has_quotes, flags[1] = has_wildcard
int	handle_word(char *line, int *i, t_token **tokens,
			t_minishell *minishell)
{
	int		flags[2];
	int		disable_expand;
	char	*full_word;
	char	*segment;

	flags[0] = FALSE;
	flags[1] = FALSE;
	disable_expand = last_token_is_heredoc(*tokens);
	full_word = init_word(line, i, minishell, disable_expand);
	while (line[*i] && !is_separator(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			flags[0] = TRUE;
			segment = handle_quoted_string(line, i, minishell, disable_expand);
		}
		else
		{
			if (!disable_expand && line[*i] == '$'
				&& (line[*i + 1] == '"' || line[*i + 1] == '\''))
			{
				(*i)++;
				continue ;
			}
			segment = get_unquoted_segment(line, i, minishell, disable_expand);
			if (!disable_expand && segment && (ft_strchr(segment, '*')
					|| ft_strchr(segment, '?')))
				flags[1] = TRUE;
		}
		full_word = strjoin_free(full_word, segment);
		if (!full_word)
			return (FALSE);
	}
	create_and_add_token(full_word, flags, tokens);
	free(full_word);
	(*i)--;
	return (TRUE);
}
