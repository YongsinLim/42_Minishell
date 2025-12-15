/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2025/11/05 00:03:44 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_word(char *line, int *i, t_token **tokens)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t'
		&& line[*i] != '|' && line[*i] != '<' && line[*i] != '>')
		(*i)++;
	word = ft_substr(line, start, *i - start);
	if (word && word[0] != '\0')
	{
		add_token(tokens, new_token(word, TOKEN_WORD));
	}
	free(word);
	(*i)--;
}
