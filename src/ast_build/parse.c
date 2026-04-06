/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 10:02:43 by yolim             #+#    #+#             */
/*   Updated: 2026/04/06 12:44:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*parse(t_token **tokens)
{
	t_ast_node		*left_node;
	t_ast_node		*op_node;
	t_ast_node_type	op_type;

	left_node = parse_pipeline(tokens);
	if (!left_node)
		return (NULL);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		op_type = set_operator_type(tokens);
		op_node = create_new_ast_node(op_type);
		if (!op_node)
			return (free_ast(&left_node), NULL);
		op_node->left = left_node;
		op_node->right = parse_pipeline(tokens);
		if (!op_node->right)
			return (free_ast(&op_node), NULL);
		left_node = op_node;
	}
	return (left_node);
}

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left_cmd;
	t_ast_node	*pipe_node;
	t_token		*current_token;

	left_cmd = parse_command_or_subshell(tokens);
	if (!left_cmd)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		current_token = *tokens;
		*tokens = current_token->next;
		free(current_token->value);
		free(current_token);
		pipe_node = create_new_ast_node(NODE_PIPE);
		if (!pipe_node)
			return (free_ast(&left_cmd), NULL);
		pipe_node->left = left_cmd;
		pipe_node->right = parse_command_or_subshell(tokens);
		if (!pipe_node->right)
			return (free_ast(&pipe_node), NULL);
		left_cmd = pipe_node;
	}
	return (left_cmd);
}

t_ast_node	*create_new_ast_node(t_ast_node_type type)
{
	t_ast_node	*node;

	node = malloc (sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->command = NULL;
	return (node);
}

t_ast_node_type	set_operator_type(t_token **tokens)
{
	t_ast_node_type	operation_type;
	t_token			*operation_token;

	operation_token = *tokens;
	if (operation_token->type == TOKEN_AND)
		operation_type = NODE_AND;
	else
		operation_type = NODE_OR;
	*tokens = operation_token->next;
	free(operation_token->value);
	free(operation_token);
	return (operation_type);
}
