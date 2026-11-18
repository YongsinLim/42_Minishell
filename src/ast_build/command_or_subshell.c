/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_or_subshell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:39:54 by yolim             #+#    #+#             */
/*   Updated: 2026/04/18 13:31:40 by yolim            ###   ########.fr       */
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
		return (parse_command(tokens));
}

static void	consume_one_token(t_token **tokens)
{
	t_token	*current_token;

	current_token = *tokens;
	*tokens = (*tokens)->next;
	free(current_token->value);
	free(current_token);
}

t_ast_node	*parse_subshell(t_token **tokens)
{
	t_ast_node	*inner_ast;
	t_ast_node	*node;

	consume_one_token(tokens);
	inner_ast = parse(tokens);
	if (!inner_ast)
		return (NULL);
	if (!*tokens || (*tokens)->type != TOKEN_R_PAREN)
	{
		ft_putstr_fd("minishell: syntax error unclosed parenthesis\n", 2);
		return (free_ast(&inner_ast), NULL);
	}
	consume_one_token(tokens);
	node = create_new_ast_node(NODE_SUBSHELL);
	if (!node)
		return (free_ast(&inner_ast), NULL);
	node->left = inner_ast;
	node->command = init_cmd();
	if (!node->command
		|| parse_redirection(tokens, node->command) == SHELL_FAILURE)
		return (free_ast(&node), NULL);
	return (node);
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;

	if (!tokens || !*tokens)
		return (ft_putstr_fd("minishell: syntax error near unexpected"
				" token 'newline'\n", 2), NULL);
	if ((*tokens)->type == TOKEN_PIPE || (*tokens)->type == TOKEN_AND
		|| (*tokens)->type == TOKEN_OR || (*tokens)->type == TOKEN_R_PAREN)
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

t_ast_node	*create_new_ast_node(t_ast_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->command = NULL;
	return (node);
}
