/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 19:11:31 by yolim             #+#    #+#             */
/*   Updated: 2026/02/04 18:58:29 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
// input = `ls` -> `>` -> `out.c`
// output =
// assumption, TOKEN_L_PAREN & TOKEN_R_PAREN will not call this function, call subshell instead
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
		// TODO: create strcut status
		status = tokens_to_cmd(&token, cmd, &argv_list);
		if (status == 1) // status != SUCCESS
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
		// TODO: revisit
		return (parse_redirection(token_ptr, cmd));
	if ((*token_ptr)->type == TOKEN_WORD
		|| (*token_ptr)->type == TOKEN_QUOTED_STRING)
	{
		value_dup = ft_strdup((*token_ptr)->value);
		if (!value_dup)
			return (1);
		ft_lstadd_back(argv_list, ft_lstnew(value_dup));
		(*token_ptr) = (*token_ptr)->next;
		return (0);
	}
}

void	*parse_error_cleanup(t_list **argv_list, t_command *cmd, char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_lstclear(argv_list, free);
	free(cmd);
	return (NULL);
}

t_command	*init_cmd(void)
{
	t_command	*command_list;

	command_list = malloc(sizeof(t_command));
	if (!command_list)
		return (NULL);
	command_list->argv = NULL;
	command_list->input_file = NULL;
	command_list->output_file = NULL;
	command_list->is_append = 0;
	command_list->heredoc_delimiter = NULL;
	command_list->heredoc_fd = -1;
	return (command_list);
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



















int	parse_redirection(t_token **tokens, t_command *cmd)
{
	t_token	*current_token;
	t_token	*filename_token;

	current_token = *tokens;
	while (current_token && (current_token->type == TOKEN_REDIRECT_IN
			|| current_token->type == TOKEN_REDIRECT_OUT
			|| current_token->type == TOKEN_APPEND
			|| current_token->type == TOKEN_HEREDOC))
	{
		filename_token = token_redirection(current_token, cmd);
		// todo hander this error code
		if (filename_token == NULL)
			return (1);
		current_token = filename_token->next;
	}
	*tokens = current_token;
	return (0);
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
