/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 12:31:16 by yolim             #+#    #+#             */
/*   Updated: 2026/03/31 22:47:51 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next_node;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		next_node = current->next;
		free(current->value);
		free(current);
		current = next_node;
	}
	*tokens = NULL;
}

void	free_array_str(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_command(t_command *cmd)
{
	t_redir	*current;
	t_redir	*next;

	if (!cmd)
		return ;
	free_array_str(cmd->argv);
	current = cmd->redirs;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
	free(cmd->heredoc_delimiter);
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	free(cmd);
}

void	free_ast(t_ast_node **ast_ptr)
{
	t_ast_node	*ast;

	if (!ast_ptr || !*ast_ptr)
		return ;
	ast = *ast_ptr;
	free_ast(&(ast->left));
	free_ast(&(ast->right));
	if (ast->type == NODE_COMMAND && ast->command)
		free_command(ast->command);
	free(ast);
	*ast_ptr = NULL;
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}
