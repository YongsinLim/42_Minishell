/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tokenising.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:18:29 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/09 16:34:09 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*syntax_error_token(t_token **tokens,
	t_minishell *minishell, char *msg)
{
	ft_putstr_fd(msg, 2);
	minishell->last_exit_status = SYNTAX_ERROR;
	free_tokens(tokens);
	return (NULL);
}

t_token	*tokenize(char *line, t_minishell *minishell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	if (has_unclosed_quotes(line))
		return (syntax_error_token(&tokens, minishell,
				"minishell: syntax error: unclosed quote\n"));
	i = -1;
	while (line[++i])
	{
		skip_invalid_char(line, &i);
		if (!line[i])
			break ;
		if (line[i] == '&' && line[i + 1] != '&')
			return (syntax_error_token(&tokens, minishell,
					"minishell: syntax error near unexpected token `&'\n"));
		if ((line[i] == '&' && line[i + 1] == '&') || line[i] == '|'
			|| line[i] == '(' || line[i] == ')')
			add_token(&tokens, make_token(&line[i], &i));
		else if (line[i] == '<' || line[i] == '>')
			add_redirection_token(line, &i, &tokens);
		else if (!handle_word(line, &i, &tokens, minishell))
			return (free_tokens(&tokens), NULL);
	}
	return (tokens);
}

void	skip_invalid_char(char *line, int *i)
{
	while (line[*i]
		&& (line[*i] == ' ' || line[*i] == '\t'
			|| (line[*i] == '$' && (line[*i + 1] == '"'
					|| line[*i + 1] == '\''))))
		(*i)++;
}

/* Treat >| like output redirection (force clobber in sh), used to
	force-overwrite a file
*/
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
