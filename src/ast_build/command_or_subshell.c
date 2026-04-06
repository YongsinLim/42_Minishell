/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_or_subshell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:39:54 by yolim             #+#    #+#             */
/*   Updated: 2026/04/06 12:43:41 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*parse_command_or_subshell(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_L_PAREN)
		return (parse_subshell(tokens));
	else
		return (parse_simple_command(tokens));
}

t_ast_node	*parse_subshell(t_token **tokens)
{
	t_ast_node	*node;
	t_ast_node	*inner_ast;
	t_token		*current_token;

	current_token = *tokens;
	*tokens = (*tokens)->next;
	free(current_token->value);
	free(current_token);
	inner_ast = parse_pipeline(tokens);
	if (!inner_ast)
		return (printf("minishell :  syntax error near '('\n"), NULL);
	current_token = *tokens;
	if (!current_token || current_token->type != TOKEN_R_PAREN)
	{
		ft_putstr_fd("minishell : syntax error unclosed parenthesis\n", 2);
		return (free_ast(&inner_ast), NULL);
	}
	*tokens = (*tokens)->next;
	free(current_token->value);
	free(current_token);
	node = create_new_ast_node(NODE_SUBSHELL);
	if (!node)
		return (free_ast(&inner_ast), NULL);
	node->left = inner_ast;
	return (node);
}

t_ast_node	*parse_simple_command(t_token **tokens)
{
	t_ast_node	*node;

	if (!tokens || !*tokens)
		return (ft_putstr_fd("minishell: syntax error near unexpected"
				"token 'newline'\n", 2), NULL);
	if ((*tokens)->type == TOKEN_PIPE || (*tokens)->type == TOKEN_AND
		|| (*tokens)->type == TOKEN_OR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		ft_putstr_fd((*tokens)->value, 2);
		ft_putstr_fd("'\n", 2);
		return (NULL);
	}
	node = create_new_ast_node(NODE_COMMAND);
	if (!node)
		return (NULL);
	node->command = parse_one_command(tokens);
	if (!node->command)
	{
		free_ast(&node);
		return (NULL);
	}
	return (node);
}
