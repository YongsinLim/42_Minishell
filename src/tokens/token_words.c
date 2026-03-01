/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/01 19:54:54 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Check if char is a separator
int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

void	handle_word(char *line, int *i, t_token **tokens,
	t_minishell *minishell)
{
	int		start;
	char	*word;
	char	*expanded_word;

	start = *i;
	while (line[*i] && !is_separator(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	expanded_word = expand_variable(word, minishell);
	if (word && word[0] != '\0')
	{
		add_token(tokens, new_token(expanded_word, TOKEN_WORD));
	}
	free(word);
	free(expanded_word);
	(*i)--;
}
