/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 10:45:19 by yolim             #+#    #+#             */
/*   Updated: 2026/03/01 19:55:14 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quoted_string(char *line, int *i, t_token **tokens,
		t_minishell *minishell)
{
	int		start_idx;
	char	quote_char;
	char	*quoted_str;
	char	*expanded_str;

	quote_char = line[*i];
	start_idx = *i + 1;
	(*i)++;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (1);
	}
	quoted_str = ft_substr(line, start_idx, *i - start_idx);
	if (quote_char == '"')
	{
		expanded_str = expand_variable(quoted_str, minishell);
		add_token(tokens, new_token(expanded_str, TOKEN_QUOTED_STRING));
		free(expanded_str);
	}
	else if (quote_char == '\'')
		add_token(tokens, new_token(quoted_str, TOKEN_QUOTED_STRING));
	return (free(quoted_str), 0);
}

char *remove_quotes(char *str)
{
    int     i;
    int     j;
    char    *new_str;
    char    quote;

    if (!str)
        return (NULL);
    new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
    if (!new_str)
        return (NULL);
    i = 0;
    j = 0;
    quote = 0;
    while (str[i])
    {
        // If we hit a quote and we are NOT inside one, enter quote state
        if (!quote && (str[i] == '\'' || str[i] == '\"'))
            quote = str[i];
        // If we hit a quote and it matches the current one, exit quote state
        else if (quote && str[i] == quote)
            quote = 0;
        // Otherwise, copy the character
        else
            new_str[j++] = str[i];
        i++;
    }
    return (new_str);
}
