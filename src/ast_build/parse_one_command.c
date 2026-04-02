/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_one_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 19:11:31 by yolim             #+#    #+#             */
/*   Updated: 2026/04/02 15:28:58 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Usage of parse_one_command :

input = `echo hello` `>` `outfile`
output =
typedef struct s_command
{
	char				**argv; = [echo, hello]
	t_redir				*redirs = NULL
	char				*heredoc_delimiter; = NULL
	int					heredoc_is_quoted; = 0
	int					heredoc_fd = -1
}			t_command;
*/

t_command	*parse_one_command(t_token **tokens_ptr)
{
	t_token		*token;
	t_command	*cmd;
	t_list		*argv_list;
	int			status;

	token = *tokens_ptr;
	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	argv_list = NULL;
	while (token && token->type != TOKEN_PIPE && token->type != TOKEN_AND
		&& token->type != TOKEN_OR)
	{
		status = tokens_to_cmd(&token, cmd, &argv_list);
		if (status == SHELL_FAILURE)
			return (parse_error_cleanup(&argv_list, cmd, NULL));
	}
	cmd->argv = to_str_array(argv_list);
	if (!cmd->argv)
		return (parse_error_cleanup(&argv_list, cmd,
				"minishell: malloc error\n"));
	*tokens_ptr = token;
	return (cmd);
}

int	add_argv_value(t_list **argv_list, char *value)
{
	char	*value_dup;
	t_list	*new_node;

	value_dup = ft_strdup(value);
	if (!value_dup)
		return (SHELL_FAILURE);
	new_node = ft_lstnew(value_dup);
	if (!new_node)
	{
		free(value_dup);
		return (SHELL_FAILURE);
	}
	ft_lstadd_back(argv_list, new_node);
	return (SHELL_SUCCESS);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

int	split_unquoted_word_to_argv(char *value, t_list **argv_list)
{
	int	i;
	int	start;
	char *field;

	i = 0;
	while (value[i])
	{
		while (value[i]  && is_whitespace(value[i]))
			i++;
		if (!value[i])
			break ;
		start = i;
		while (value[i]  && !is_whitespace(value[i]))
			i++;
		field = ft_substr(value, start, i - start);
		if (!field || add_argv_value(argv_list, field) == SHELL_FAILURE)
		{
			free(field);
			return (SHELL_FAILURE);
		}
		free(field);
	}
	return (SHELL_SUCCESS);
}

int	tokens_to_cmd(t_token **token_ptr, t_command *cmd, t_list **argv_list)
{
	t_list *expanded;
	t_list *tail;

	if ((*token_ptr)->type == TOKEN_REDIRECT_OUT
		|| (*token_ptr)->type == TOKEN_REDIRECT_IN
		|| (*token_ptr)->type == TOKEN_HEREDOC
		|| (*token_ptr)->type == TOKEN_APPEND)
		return (parse_redirection(token_ptr, cmd));
	if ((*token_ptr)->type == TOKEN_WORD)
	{
		if ((*token_ptr)->has_wildcard)
		{
			expanded = NULL;
			expand_wildcard((*token_ptr)->value, &expanded);
			if (!*argv_list)
				*argv_list = expanded;
			else
			{
				tail = ft_lstlast(*argv_list);
				tail->next = expanded;
			}
		}
		else if ((*token_ptr)->has_quotes == 0 && ft_strchr((*token_ptr)->value, ' '))
		{
			if (split_unquoted_word_to_argv((*token_ptr)->value, argv_list) == SHELL_FAILURE)
				return (SHELL_FAILURE);
		}
		else
		{
			if (add_argv_value(argv_list, (*token_ptr)->value) == SHELL_FAILURE)
				return (SHELL_FAILURE);
		}
		(*token_ptr) = (*token_ptr)->next;
	}
	return (SHELL_SUCCESS);
}

int	parse_redirection(t_token **tokens, t_command *cmd)
{
	t_token	*current_token;
	t_token	*token_filename;

	current_token = *tokens;
	while (current_token && (current_token->type == TOKEN_REDIRECT_IN
			|| current_token->type == TOKEN_REDIRECT_OUT
			|| current_token->type == TOKEN_HEREDOC
			|| current_token->type == TOKEN_APPEND))
	{
		token_filename = token_redirection(current_token, cmd);
		if (token_filename == NULL)
			return (SHELL_FAILURE);
		current_token = token_filename->next;
	}
	*tokens = current_token;
	return (SHELL_SUCCESS);
}

void	add_redir(t_command *cmd, t_token_type type, char *file)
{
	t_redir	*new_node;
	t_redir	*current;

	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return ;
	new_node->type = type;
	new_node->file = ft_strdup(file);
	new_node->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new_node;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

t_token	*token_redirection(t_token *token, t_command *command)
{
	t_token_type	type;

	type = token->type;
	token = token->next;
	if (token == NULL)
	{
		ft_putstr_fd("minishell : syntax error near unexpected token 'newline'\n", 2);
		return (NULL);
	}
	if (token->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell : syntax error near unexpected token '", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
		return (NULL);
	}
	if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND || type == TOKEN_REDIRECT_IN)
		add_redir(command, type, token->value);
	else if (type == TOKEN_HEREDOC)
	{
		command->heredoc_delimiter = ft_strdup(token->value);
		command->heredoc_is_quoted = token->has_quotes;
	}
	return (token);
}

char	**to_str_array(t_list *argv_list)
{
	char	**argv;
	int		i;
	t_list	*current;

	argv = (char **)malloc(sizeof(char *) * (ft_lstsize(argv_list) + 1));
	if (!argv)
		return (NULL);
	i = 0;
	current = argv_list;
	while (current != NULL)
	{
		argv[i] = (char *)current->content;
		i++;
		current = current->next;
	}
	argv[i] = NULL;
	ft_lstclear(&argv_list, NULL);
	return (argv);
}
