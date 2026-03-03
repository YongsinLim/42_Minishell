/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:32:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/03 18:17:54 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Check if char is a separator
int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

char	*expand_tilde(char *str, t_minishell *minishell)
{
	char	*home;
	char	*expanded;

	if (!str)
		return (NULL);
	if (str[0] == '~' && (str[1] == '\0' || str[1] == '/'))
	{
		home = get_var_value("HOME", minishell);
		if (!home || home[0] == '\0')
		{
			free(home);
			return (ft_strdup(str));
		}
		if (str[1] == '\0')
			return (home);
		expanded = ft_strjoin(home, &str[1]);
		free(home);
		return (expanded);
	}
	return (ft_strdup(str));
}

void	handle_word(char *line, int *i, t_token **tokens,
	t_minishell *minishell)
{
	int		start;
	char	*word;
	char	*tilde_expanded;
	char	*expanded_word;

	start = *i;
	while (line[*i] && !is_separator(line[*i]))
		(*i)++;
	word = ft_substr(line, start, *i - start);
	tilde_expanded = expand_tilde(word, minishell);
	expanded_word = expand_variable(tilde_expanded, minishell);
	if (word && word[0] != '\0')
	{
		add_token(tokens, new_token(expanded_word, TOKEN_WORD));
	}
	free(word);
	free(tilde_expanded);
	free(expanded_word);
	(*i)--;
}
