/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:21:59 by jenlee            #+#    #+#             */
/*   Updated: 2026/01/30 17:34:16 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

t_token	*make_token(char *value, int *i)
{
	if (*value == '&' && *(value + 1) == '&')
	{
		(*i)++;
		return (new_token("&&", TOKEN_AND));
	}
	else if (*value == '|')
	{
		if (*(value + 1) == '|')
		{
			(*i)++;
			return (new_token("||", TOKEN_OR));
		}
		else
			return (new_token("|", TOKEN_PIPE));
	}
	else if (*value == '(')
		return (new_token("(", TOKEN_L_PAREN));
	else if (*value == ')')
		return (new_token(")", TOKEN_R_PAREN));
	return (NULL);
}

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
