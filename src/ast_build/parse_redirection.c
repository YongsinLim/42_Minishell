/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 11:39:19 by yolim             #+#    #+#             */
/*   Updated: 2026/04/17 11:23:50 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_redirection(t_token **tokens, t_command *cmd)
{
	t_token	*op_token;
	t_token	*file_token;

	while (*tokens && is_redirection_token(*tokens))
	{
		if (set_redirection_file(*tokens, cmd) == SHELL_FAILURE)
			return (SHELL_FAILURE);
		op_token = *tokens;
		file_token = (*tokens)->next;
		*tokens = file_token->next;
		free(op_token->value);
		free(op_token);
		free(file_token->value);
		free(file_token);
	}
	return (SHELL_SUCCESS);
}

int	set_redirection_file(t_token *token, t_command *command)
{
	t_token	*target;
	t_redir	*curr;

	if (get_redirection_file(&target, token) == SHELL_FAILURE)
		return (SHELL_FAILURE);
	restore_quoted_whitespaces(target->value);
	restore_wildcards_str(target->value);
	if (add_redir(command, token->type, target->value) == SHELL_FAILURE)
		return (SHELL_FAILURE);
	if (token->type == TOKEN_HEREDOC)
	{
		curr = command->redirs;
		while (curr && curr->next)
			curr = curr->next;
		if (curr)
			curr->is_quoted = target->has_quotes;
	}
	return (SHELL_SUCCESS);
}

int	get_redirection_file(t_token **target, t_token *token)
{
	*target = token->next;
	if (!*target)
	{
		ft_putstr_fd("minishell : syntax error near unexpected token "
			"'newline'\n", 2);
		return (SHELL_FAILURE);
	}
	if ((*target)->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell : syntax error near unexpected token '", 2);
		ft_putstr_fd((*target)->value, 2);
		ft_putstr_fd("'\n", 2);
		return (SHELL_FAILURE);
	}
	return (SHELL_SUCCESS);
}

void	restore_quoted_whitespaces(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\x1A')
			str[i] = ' ';
		else if (str[i] == '\x1B')
			str[i] = '\t';
		else if (str[i] == '\x1C')
			str[i] = '\n';
		i++;
	}
}

int	add_redir(t_command *cmd, t_token_type type, char *file)
{
	t_redir	*new_node;
	t_redir	*current;

	new_node = ft_calloc(1, sizeof(t_redir));
	if (!new_node)
		return (SHELL_FAILURE);
	new_node->type = type;
	new_node->heredoc_fd = -1;
	new_node->file = ft_strdup(file);
	if (!new_node->file)
		return (free(new_node), SHELL_FAILURE);
	if (!cmd->redirs)
		cmd->redirs = new_node;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (SHELL_SUCCESS);
}
