/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/01/31 12:24:51 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Check if char is a separator
int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

void	handle_word(char *line, int *i, t_token **tokens)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] && is_separator(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	if (word && word[0] != '\0')
	{
		add_token(tokens, new_token(word, TOKEN_WORD));
	}
	free(word);
	(*i)--;
}
