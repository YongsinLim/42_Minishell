/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tokenising.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:18:29 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/28 15:30:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_invalid_char(char *line, int *i)
{
	while (line[*i]
		&& (line[*i] == ' ' || line[*i] == '\t'
			|| (line[*i] == '$' && (line[*i + 1] == '"'
					|| line[*i + 1] == '\''))))
		(*i)++;
}

void	add_redirection_token(char *line, int *i, t_token **tokens)
{
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			add_token(tokens, new_token(">>", TOKEN_APPEND, FALSE));
			(*i)++;
		}
		else if (line[*i + 1] == '|')
		{
			/* Treat >| like output redirection (force clobber in sh). */
			add_token(tokens, new_token(">", TOKEN_REDIRECT_OUT, FALSE));
			(*i)++;
		}
		else
			add_token(tokens, new_token(">", TOKEN_REDIRECT_OUT, FALSE));
	}
	else if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			add_token(tokens, new_token("<<", TOKEN_HEREDOC, FALSE));
			(*i)++;
		}
		else
			add_token(tokens, new_token("<", TOKEN_REDIRECT_IN, FALSE));
	}
}

t_token	*tokenize(char *line, t_minishell *minishell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_invalid_char(line, &i);
		if (!line[i])
			break ;
		if ((line[i] == '&' && line[i + 1] == '&') || line[i] == '|'
			|| line[i] == '(' || line[i] == ')')
			add_token(&tokens, make_token(&line[i], &i));
		else if (line[i] == '<' || line[i] == '>')
			add_redirection_token(line, &i, &tokens);
		else
			handle_word(line, &i, &tokens, minishell);
		i++;
	}
	return (tokens);
}
