/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tokenising.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:18:29 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/22 18:06:48 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_spaces(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

void	add_redirection_token(char *line, int *i, t_token **tokens)
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

/*t_token *tokenize(char *line, t_env *env)
{
    t_token *tokens;
    int     i;

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
            add_redirection_token(line, &i, &tokens);
        else if (line[i] == '"' || line[i] == '\'')
        {
            if (handle_quoted_string(line, &i, &tokens, env) == 1)
                return (free_tokens(&tokens), NULL); // TODO: handle unclosed quotes
        }
        else
            handle_word(line, &i, &tokens);
		i++;
	}
    return (tokens);
}*/

t_token *tokenize(char *line, t_env *env)
{
    t_token *tokens;
    t_token *tmp;
    char    *expanded;
    int     i;

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
            add_redirection_token(line, &i, &tokens);
        // NO MORE SPECIAL QUOTE HANDLING HERE! handle_word does it now.
        else
            handle_word(line, &i, &tokens);
		i++;
	}

    // POST-PROCESSING: Expand variables BEFORE quotes are removed
    tmp = tokens;
    while (tmp)
    {
        if (tmp->type == TOKEN_WORD)
        {
            expanded = expand_variable(tmp->value, env);
            free(tmp->value);
            tmp->value = expanded;
        }
        tmp = tmp->next;
    }

    return (tokens);
}
