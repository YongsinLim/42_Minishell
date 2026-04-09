/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:21:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/09 16:30:33 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	tmp = *head;
	if (!tmp)
	{
		*head = new_token;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

t_token	*make_token(char *value, int *i)
{
	if (*value == '&' && *(value + 1) == '&')
	{
		(*i)++;
		return (new_token("&&", TOKEN_AND, FALSE));
	}
	if (*value == '|')
	{
		if (*(value + 1) == '|')
		{
			(*i)++;
			return (new_token("||", TOKEN_OR, FALSE));
		}
		return (new_token("|", TOKEN_PIPE, FALSE));
	}
	if (*value == '(')
		return (new_token("(", TOKEN_L_PAREN, FALSE));
	if (*value == ')')
		return (new_token(")", TOKEN_R_PAREN, FALSE));
	return (NULL);
}

t_token	*new_token(char *value, t_token_type type, int has_quotes)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->has_quotes = has_quotes;
	token->next = NULL;
	return (token);
}
