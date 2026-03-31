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

// Helper to print indentation
void	print_indent(int level)
{
	while (level > 0)
	{
		ft_printf("    "); // 4 spaces
		level--;
	}
}

void	print_ast(t_ast_node *node, int level)
{
	int		i;
	t_redir	*current_redir;

	if (!node)
	{
		ft_putstr_fd("AST is NULL\n", 1);
		return ;
	}
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
	{
		ft_printf("CMD: ");
		i = 0;
		if (node->command && node->command->argv)
		{
			while (node->command->argv[i])
			{
				ft_printf("'%s' ", node->command->argv[i]);
				i++;
			}
			// Print all redirections from the new linked list
			current_redir = node->command->redirs;
			while (current_redir)
			{
				if (current_redir->type == TOKEN_REDIRECT_IN)
					ft_printf(" < %s", current_redir->file);
				else if (current_redir->type == TOKEN_REDIRECT_OUT)
					ft_printf(" > %s", current_redir->file);
				else if (current_redir->type == TOKEN_APPEND)
					ft_printf(" >> %s", current_redir->file);
				current_redir = current_redir->next;
			}
			if (node->command->heredoc_delimiter)
				ft_printf(" << %s", node->command->heredoc_delimiter);
		}
		else
			ft_printf("Unknown Node Type\n");
		ft_printf("\n");
	}
	if (node->left)
	{
		print_indent(level);
		ft_printf("Left:\n");
		print_ast(node->left, level + 1);
	}
	if (node->right)
	{
		print_indent(level);
		ft_printf("Right:\n");
		print_ast(node->right, level + 1);
	}
}
