/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tokenising.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:18:29 by jenlee            #+#    #+#             */
/*   Updated: 2026/01/30 17:39:02 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_spaces(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

void	handle_redirection(char *line, int *i, t_token **tokens)
{
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			add_token(tokens, new_token(">>", TOKEN_APPEND));
			(*i)++;
		}
		else
			add_token(tokens, new_token(">", TOKEN_REDIRECT_OUT));
	}
	else if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			add_token(tokens, new_token("<<", TOKEN_HEREDOC));
			(*i)++;
		}
		else
			add_token(tokens, new_token("<", TOKEN_REDIRECT_IN));
	}
}

t_token	*tokenize(char *line, char **envp)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_spaces(line, &i);
		if (!line[i])
			break ;
		else if ((line[i] == '&' && line[i + 1] == '&') || line[i] == '|'
			|| line[i] == '(' || line[i] == ')')
			add_token(&tokens, make_token(&line[i], &i));
		else if (line[i] == '<' || line[i] == '>')
			handle_redirection(line, &i, &tokens);
		else if (line[i] == '"' || line[i] == '\'')
		{
			if (handle_quoted_string(line, &i, &tokens, envp) == 1)
				return (free_tokens(&tokens), NULL); // TODO: handle unclosed quotes
		}
		else
			handle_word(line, &i, &tokens);
		i++;
	}
	return (tokens);
}
