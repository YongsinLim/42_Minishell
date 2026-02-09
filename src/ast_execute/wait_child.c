/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:46:24 by yolim             #+#    #+#             */
/*   Updated: 2026/02/09 17:23:55 by yolim            ###   ########.fr       */
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

/*
waitpid(-1, &status, 0)
-1 : tells the shell to pause and wait for ANY child process to terminate.
		It returns the PID of the child that just finished.
status: A pointer to an integer where the child's status information is stored.
		Passing NULL discards this information.
0 : Option

`waitpid` returns -1 when there are no more child processes to wait for.

WIFEXITED checks if the child terminated normally (e.g., with exit() return).

WEXITSTATUS extracts the actual exit code (e.g., 0, 1, 127)

WIFSIGNALED checks if the child was terminated by a signal
	(e.g., segmentation fault or Ctrl+C)

WTERMSIG gets the signal number (start from 128 onwards)
*/

void	error_exit(char *error_msg)
{
	perror(error_msg);
	exit (SHELL_FAILURE);
}
