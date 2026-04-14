/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 14:20:02 by yolim             #+#    #+#             */
/*   Updated: 2026/04/14 15:08:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	report_error(char *msg, char *param)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	if (param)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(param, 2);
	}
	ft_putstr_fd("\n", 2);
}

void	report_chdir_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

// ---------------------------------------------------------------------









void	error_exit(char *error_msg)
{
	perror(error_msg);
	exit (SHELL_FAILURE);
}

char	*exit_status(t_minishell *minishell, char *cmd_name, char *message,
	int exit_status)
{
	report_error(cmd_name, message);
	minishell->last_exit_status = exit_status;
	return (NULL);
}
