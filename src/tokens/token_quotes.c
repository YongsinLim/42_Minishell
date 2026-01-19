/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 10:45:19 by yolim             #+#    #+#             */
/*   Updated: 2026/01/13 19:19:42 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quoted_string(char *line, int *i, t_token **tokens, char **envp)
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
		ft_putstr_fd("minishell: unclosed quote.\n", 2);
		return (1);
	}
	quoted_str = ft_substr(line, start_idx, *i - start_idx);
	if (quote_char == '"')
	{
		expanded_str = expand_variable(quoted_str, envp);
		add_token(tokens, new_token(expanded_str, TOKEN_QUOTED_STRING));
		free(expanded_str);
	}
	else if (quote_char == '\'')
		add_token(tokens, new_token(quoted_str, TOKEN_QUOTED_STRING));
	return (free(quoted_str), (*i)++, 0);
}
