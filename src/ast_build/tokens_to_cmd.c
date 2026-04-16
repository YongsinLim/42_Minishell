/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:02:43 by yolim             #+#    #+#             */
/*   Updated: 2026/04/13 11:35:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_word_token(t_token *token, t_list **argv_list)
{
	t_list	*temp_split;
	t_list	*current;
	t_list	*expanded;

	if (build_word_field(&temp_split, token) == SHELL_FAILURE)
		return (SHELL_FAILURE);
	if (token->has_wildcard)
	{
		current = temp_split;
		while (current)
		{
			expanded = NULL;
			expand_wildcard((char *)current->content, &expanded);
			ft_lstadd_back(argv_list, expanded);
			current = current->next;
		}
		ft_lstclear(&temp_split, free);
	}
	else
		ft_lstadd_back(argv_list, temp_split);
	return (SHELL_SUCCESS);
}

static void	free_and_advance_token(t_token **token_ptr)
{
	t_token	*to_free;

	to_free = *token_ptr;
	*token_ptr = (*token_ptr)->next;
	free(to_free->value);
	free(to_free);
}

/*
Quoted empty tokens must be kept as real argv entries.
echo "" -> argv includes empty string
echo $EMPTY (unquoted, empty var) -> no empty arg added
echo "$EMPTY" -> empty arg preserved

Why wildcard need iteration, non-wildcard no need :
Non-wildcard: data already final -> attach whole linked-list once.
Wildcard: each node is a pattern needing its own transform -> must visit
each node (current = current->next).
 */
int	tokens_to_cmd(t_token **token_ptr, t_list **argv_list)
{
	if ((*token_ptr)->type == TOKEN_WORD)
	{
		if (handle_word_token(*token_ptr, argv_list) == SHELL_FAILURE)
			return (SHELL_FAILURE);
		free_and_advance_token(token_ptr);
	}
	return (SHELL_SUCCESS);
}
