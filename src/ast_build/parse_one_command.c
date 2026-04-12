/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_one_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 19:11:31 by yolim             #+#    #+#             */
/*   Updated: 2026/04/12 16:22:19 by yolim            ###   ########.fr       */
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
	while (is_not_logical_operator(token))
	{
		if (is_redirection_token(token))
			status = parse_redirection(&token, cmd);
		else
			status = tokens_to_cmd(&token, &argv_list);
		if (status == SHELL_FAILURE)
			return (parse_error_cleanup(&argv_list, cmd, NULL));
	}
	cmd->argv = convert_list_to_str_array(argv_list);
	if (!cmd->argv)
		return (parse_error_cleanup(&argv_list, cmd,
				"minishell: malloc error\n"));
	*tokens_ptr = token;
	return (cmd);
}

/*
Quoted empty tokens must be kept as real argv entries.
echo "" -> argv includes empty string
echo $EMPTY (unquoted, empty var) -> no empty arg added
echo "$EMPTY" -> empty arg preserved

Why wildcard need iteration, non-wildcard no need :
Non-wildcard: data already final -> attach whole linked-list once.
Wildcard: each node is a pattern needing its own transform -> must visit
each node (current = current->next).
 */
int	tokens_to_cmd(t_token **token_ptr, t_list **argv_list)
{
	t_list	*temp_split;
	t_list	*current;
	t_list	*expanded;

	if ((*token_ptr)->type == TOKEN_WORD)
	{
		if (build_word_field(&temp_split, *token_ptr) == SHELL_FAILURE)
			return (SHELL_FAILURE);
		if ((*token_ptr)->has_wildcard)
		{
			current = temp_split;
			while (current)
			{
				expanded = NULL;
				expand_wildcard((char *)current->content, &expanded);
				ft_lstadd_back(argv_list, expanded);
				current = current->next;
			}
			ft_lstclear(&temp_split, free);
		}
		else
			ft_lstadd_back(argv_list, temp_split);
		(*token_ptr) = (*token_ptr)->next;
	}
	return (SHELL_SUCCESS);
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
