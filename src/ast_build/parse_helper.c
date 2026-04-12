/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:43:13 by yolim             #+#    #+#             */
/*   Updated: 2026/04/12 16:12:29 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*init_cmd(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->argv = NULL;
	command->redirs = NULL;
	command->heredoc_delimiter = NULL;
	command->heredoc_is_quoted = FALSE;
	command->heredoc_fd = -1;
	return (command);
}

int	is_not_logical_operator(t_token *token)
{
	return (token
		&& token->type != TOKEN_PIPE
		&& token->type != TOKEN_AND
		&& token->type != TOKEN_OR);
}

int	is_redirection_token(t_token *token)
{
	if (token && (token->type == TOKEN_REDIRECT_IN
			|| token->type == TOKEN_REDIRECT_OUT
			|| token->type == TOKEN_APPEND
			|| token->type == TOKEN_HEREDOC))
		return (TRUE);
	return (FALSE);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

void	*parse_error_cleanup(t_list **argv_list, t_command *cmd, char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_lstclear(argv_list, free);
	free_command(cmd);
	return (NULL);
}
