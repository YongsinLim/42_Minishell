/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:43:13 by yolim             #+#    #+#             */
/*   Updated: 2026/02/08 18:27:04 by yolim            ###   ########.fr       */
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
	command->input_file = NULL;
	command->output_file = NULL;
	command->is_append = 0;
	command->heredoc_delimiter = NULL;
	command->heredoc_is_quoted = 0;
	command->heredoc_fd = -1;
	return (command);
}

void	*parse_error_cleanup(t_list **argv_list, t_command *cmd, char *msg)
{
	ft_putstr_fd(msg, 2);
	ft_lstclear(argv_list, free);
	free(cmd);
	return (NULL);
}
