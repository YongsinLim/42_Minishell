/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:12:07 by jenlee            #+#    #+#             */
/*   Updated: 2026/01/31 12:41:08 by yolim            ###   ########.fr       */
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
	int	i;

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
			// Optional: Print redirections
			if (node->command->input_file)
				ft_printf(" < %s", node->command->input_file);
			if (node->command->output_file)
			{
				if (node->command->is_append)
					ft_printf(" >> %s", node->command->output_file);
					// Handles append
				else
					ft_printf(" > %s", node->command->output_file);
					// Handles truncate
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
