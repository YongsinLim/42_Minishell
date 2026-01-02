/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:46:24 by yolim             #+#    #+#             */
/*   Updated: 2025/12/27 12:08:59 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	wait_for_children(pid_t last_pid)
{
	int		status;
	int		last_child_status;
	pid_t	terminated_pid;

	last_child_status = 0;
	while (1)
	{
		terminated_pid = waitpid(-1, &status, 0);
		if (terminated_pid == -1)
			break ;
		if (terminated_pid == last_pid)
		{
			if (WIFEXITED(status))
				last_child_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_child_status = 128 + WTERMSIG(status);
		}
	}
	return (last_child_status);
}

void	error_exit(char *error_msg)
{
	perror(error_msg);
	exit (1);
}
