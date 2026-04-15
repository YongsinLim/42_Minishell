/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:24:05 by yolim             #+#    #+#             */
/*   Updated: 2026/04/15 15:27:26 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_directory(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (TRUE);
	}
	return (FALSE);
}

/*
Special case: . without argument should exit with Syntax error
Special case: .. (parent directory reference) should treated as not found
Normal case: directory case
*/
char	*cmd_is_dir(char *cmd, t_minishell *minishell)
{
	if (ft_strncmp(cmd, ".", 2) == 0)
		return (report_error_exit_status(minishell, cmd,
				"filename argument required", CMD_NOT_FOUND));
	if (ft_strncmp(cmd, "..", 3) == 0)
		return (report_error_exit_status(minishell, cmd,
				"command not found", CMD_NOT_FOUND));
	return (report_error_exit_status(minishell, cmd, "Is a directory",
			CMD_NOT_EXECUTABLE));
}
