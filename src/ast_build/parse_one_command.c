/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_one_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 19:11:31 by yolim             #+#    #+#             */
/*   Updated: 2026/02/09 17:11:34 by yolim            ###   ########.fr       */
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
	char				*input_file; = NULL
	char				*output_file; = outfile
	int					is_append; = 0
	char				*heredoc_delimiter; = NULL
	int					heredoc_fd; = -1
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
			return (parse_error_cleanup(&argv_list, cmd,
					"minishell: malloc error\n"));
	}
	cmd->argv = to_str_array(argv_list);
	if (!cmd->argv)
		return (parse_error_cleanup(&argv_list, cmd,
				"minishell: malloc error\n"));
	*tokens_ptr = token;
	return (cmd);
}

int	tokens_to_cmd(t_token **token_ptr, t_command *cmd, t_list **argv_list)
{
	char	*value_dup;

	if ((*token_ptr)->type == TOKEN_REDIRECT_OUT
		|| (*token_ptr)->type == TOKEN_REDIRECT_IN
		|| (*token_ptr)->type == TOKEN_HEREDOC
		|| (*token_ptr)->type == TOKEN_APPEND)
		return (parse_redirection(token_ptr, cmd));
	if ((*token_ptr)->type == TOKEN_WORD
		|| (*token_ptr)->type == TOKEN_QUOTED_STRING)
	{
		value_dup = ft_strdup((*token_ptr)->value);
		if (!value_dup)
			return (SHELL_FAILURE);
		ft_lstadd_back(argv_list, ft_lstnew(value_dup));
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

t_token	*token_redirection(t_token *token, t_command *command)
{
	t_token_type	type;

	type = token->type;
	token = token->next;
	if (token == NULL)
	{
		ft_putstr_fd("minishell : invalid file name", 2);
		return (NULL);
	}
	if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND)
	{
		command->output_file = ft_strdup(token->value);
		command->is_append = type == TOKEN_APPEND;
	}
	else if (type == TOKEN_REDIRECT_IN)
		command->input_file = ft_strdup(token->value);
	else if (type == TOKEN_HEREDOC)
	{
		command->heredoc_delimiter = ft_strdup(token->value);
		if (token->type == TOKEN_QUOTED_STRING)
			command->heredoc_is_quoted = 1;
		else
			command->heredoc_is_quoted = 0;
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
