/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_one_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 19:11:31 by yolim             #+#    #+#             */
/*   Updated: 2026/04/17 11:24:21 by yolim            ###   ########.fr       */
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
	t_command	*cmd;
	t_list		*argv_list;
	int			status;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	argv_list = NULL;
	while (*tokens_ptr && is_not_logical_operator(*tokens_ptr))
	{
		if (is_redirection_token(*tokens_ptr))
			status = parse_redirection(tokens_ptr, cmd);
		else
			status = tokens_to_cmd(tokens_ptr, &argv_list);
		if (status == SHELL_FAILURE)
			return (parse_error_cleanup(&argv_list, cmd, NULL));
	}
	cmd->argv = convert_list_to_str_array(argv_list);
	if (!cmd->argv)
		return (parse_error_cleanup(&argv_list, cmd,
				"minishell: malloc error\n"));
	return (cmd);
}

int	build_word_field(t_list	**temp_split, t_token *token)
{
	*temp_split = NULL;
	if (token->value[0] == '\0' && token->has_quotes)
	{
		if (add_argv_value(temp_split, "") == SHELL_FAILURE)
			return (SHELL_FAILURE);
	}
	return (split_unquoted_word_to_argv(token->value, temp_split));
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

/*
Restore whitespace after field splitting bcos quoted spaces could get split
incorrectly.
*/
int	split_unquoted_word_to_argv(char *value, t_list **argv_list)
{
	int		i;
	int		start_idx;
	char	*field;

	i = 0;
	while (value[i])
	{
		while (value[i] && is_whitespace(value[i]))
			i++;
		if (!value[i])
			break ;
		start_idx = i;
		while (value[i] && !is_whitespace(value[i]))
			i++;
		field = ft_substr(value, start_idx, i - start_idx);
		if (!field || add_argv_value(argv_list, field) == SHELL_FAILURE)
		{
			free(field);
			return (SHELL_FAILURE);
		}
		free(field);
	}
	return (SHELL_SUCCESS);
}
