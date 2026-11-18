/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 16:18:42 by yolim             #+#    #+#             */
/*   Updated: 2026/04/12 16:25:18 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
argv[i] points to current->content, so content must stay alive.
only free node itself
*/
char	**convert_list_to_str_array(t_list *argv_list)
{
	char	**argv;
	int		i;
	t_list	*current;
	t_list	*next;

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
	while (argv_list)
	{
		next = argv_list->next;
		free(argv_list);
		argv_list = next;
	}
	return (argv);
}

void	close_cmd_heredocs(t_command *cmd)
{
	t_redir	*current;

	if (!cmd)
		return ;
	current = cmd->redirs;
	while (current)
	{
		if (current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}
