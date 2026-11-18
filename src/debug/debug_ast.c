/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:12:07 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/31 22:49:12 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_indent(int level)
{
	while (level > 0)
	{
		ft_printf("    ");
		level--;
	}
}

static void	print_cmd_node(t_command *cmd)
{
	int		i;
	t_redir	*curr;

	ft_printf("CMD: ");
	i = 0;
	while (cmd && cmd->argv && cmd->argv[i])
		ft_printf("'%s' ", cmd->argv[i++]);
	curr = NULL;
	if (cmd)
		curr = cmd->redirs;
	while (curr)
	{
		if (curr->type == TOKEN_REDIRECT_IN)
			ft_printf(" < %s", curr->file);
		else if (curr->type == TOKEN_REDIRECT_OUT)
			ft_printf(" > %s", curr->file);
		else if (curr->type == TOKEN_APPEND)
			ft_printf(" >> %s", curr->file);
		else if (curr->type == TOKEN_HEREDOC)
			ft_printf(" << %s", curr->file);
		curr = curr->next;
	}
	ft_printf("\n");
}

void	print_ast(t_ast_node *node, int level)
{
	if (!node)
		return ((void)ft_putstr_fd("AST is NULL\n", 1));
	print_indent(level);
	if (node->type == NODE_AND)
		ft_printf("[  &&  ]\n");
	else if (node->type == NODE_OR)
		ft_printf("[  ||  ]\n");
	else if (node->type == NODE_PIPE)
		ft_printf("[  |  ]\n");
	else if (node->type == NODE_SUBSHELL)
		ft_printf("NODE_SUBSHELL\n");
	else if (node->type == NODE_COMMAND)
		print_cmd_node(node->command);
	if (node->left)
		print_ast(node->left, level + 1);
	if (node->right)
		print_ast(node->right, level + 1);
}
