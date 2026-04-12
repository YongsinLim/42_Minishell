/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_one_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 19:11:31 by yolim             #+#    #+#             */
/*   Updated: 2026/04/12 12:44:51 by yolim            ###   ########.fr       */
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
	t_redir				*redirs = type (>), file (outfile)
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


	// ---------------------------------------------------------------------




	cmd->argv = to_str_array(argv_list);
	if (!cmd->argv)
		return (parse_error_cleanup(&argv_list, cmd,
				"minishell: malloc error\n"));
	*tokens_ptr = token;
	return (cmd);
}








int	tokens_to_cmd(t_token **token_ptr, t_command *cmd, t_list **argv_list)
{
	t_list	*temp_split;
	t_list	*current;
	t_list	*expanded;

	if (is_redirection_token(*token_ptr))
		return (parse_redirection(token_ptr, cmd));

	// ---------------------------------------------------------------------


	if ((*token_ptr)->type == TOKEN_WORD)
	{
		temp_split = NULL;
		if (ft_strchr((*token_ptr)->value, ' ')
			|| ft_strchr((*token_ptr)->value, '\t')
			|| ft_strchr((*token_ptr)->value, '\n'))
		{
			if (split_unquoted_word_to_argv((*token_ptr)->value,
					&temp_split) == SHELL_FAILURE)
				return (SHELL_FAILURE);
		}
		else
		{
			if (add_argv_value(&temp_split,
					(*token_ptr)->value) == SHELL_FAILURE)
				return (SHELL_FAILURE);
		}
		if ((*token_ptr)->has_wildcard)
		{
			current = temp_split;
			while (current)
			{
				expanded = NULL;
				expand_wildcard((char *)current->content, &expanded);
				if (!*argv_list)
					*argv_list = expanded;
				else
					ft_lstlast(*argv_list)->next = expanded;
				current = current->next;
			}
			ft_lstclear(&temp_split, free);
		}
		else
		{
			if (!*argv_list)
				*argv_list = temp_split;
			else
				ft_lstlast(*argv_list)->next = temp_split;
		}
		(*token_ptr) = (*token_ptr)->next;
	}
	return (SHELL_SUCCESS);
}













int	add_argv_value(t_list **argv_list, char *value)
{
	char	*value_dup;
	t_list	*new_node;

	value_dup = ft_strdup(value);
	if (!value_dup)
		return (SHELL_FAILURE);
	restore_quoted_whitespaces(value_dup);
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
	int		i;
	int		start;
	char	*field;

	i = 0;
	while (value[i])
	{
		while (value[i] && is_whitespace(value[i]))
			i++;
		if (!value[i])
			break ;
		start = i;
		while (value[i] && !is_whitespace(value[i]))
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
