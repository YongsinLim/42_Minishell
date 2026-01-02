/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:35:36 by yolim             #+#    #+#             */
/*   Updated: 2026/01/02 18:49:24 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*parse(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;

	if (!tokens)
		return (NULL);
	head = NULL;
	current = NULL;
	while (tokens != NULL)
	{
		new_cmd = parse_one_command(&tokens);
		if (new_cmd == NULL)
			return (NULL);
		if (head == NULL)
		{
			head = new_cmd;
			current = head;
		}
		else
			current = assign_next_cmd(current, new_cmd);
		if (tokens != NULL && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}

t_command	*assign_next_cmd(t_command *current, t_command *new_cmd)
{
	current->next = new_cmd;
	current = new_cmd;
	return (current);
}

t_command	*parse_one_command(t_token **tokens_ptr)
{
	t_token		*token;
	t_command	*command_list;
	int			i;

	token = *tokens_ptr;
	command_list = initialise_command_list(count_words(token));
	if (!command_list)
		return (NULL);
	i = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_APPEND
			|| token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_HEREDOC)
		{
			token = token_redirection(token, command_list);
			if (token == NULL)
				return (free_pipeline(&command_list), NULL);
		}
		else if (token->type == TOKEN_WORD)
			command_list->argv[i++] = ft_strdup(token->value);
		token = token->next;
	}
	command_list->argv[i] = NULL;
	*tokens_ptr = token;
	return (command_list);
}

t_command	*initialise_command_list(int words_count)
{
	t_command	*command_list;

	command_list = malloc(sizeof(t_command));
	if (!command_list)
		return (NULL);
	command_list->argv = NULL;
	command_list->input_file = NULL;
	command_list->output_file = NULL;
	command_list->heredoc_delimiter = NULL;
	command_list->is_append = 0;
	command_list->heredoc_fd = -1;
	command_list->next = NULL;
	command_list->argv = malloc(sizeof(char *) * (words_count + 1));
	if (!command_list->argv)
	{
		free(command_list);
		return (NULL);
	}
	return (command_list);
}

t_token	*token_redirection(t_token *token, t_command *command_list)
{
	t_token_type	type;

	type = token->type;
	token = token->next;
	if (token == NULL)
	{
		ft_putstr_fd("minishell : invalid file name", 2);
		return (NULL);
	}
	if (type == TOKEN_REDIRECT_OUT)
	{
		command_list->output_file = ft_strdup(token->value);
		command_list->is_append = 0;
	}
	else if (type == TOKEN_APPEND)
	{
		command_list->output_file = ft_strdup(token->value);
		command_list->is_append = 1;
	}
	else if (type == TOKEN_REDIRECT_IN)
		command_list->input_file = ft_strdup(token->value);
	else if (type == TOKEN_HEREDOC)
		command_list->heredoc_delimiter = ft_strdup(token->value);
	return (token);
}
