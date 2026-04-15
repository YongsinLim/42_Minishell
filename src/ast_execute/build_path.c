/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 15:14:41 by yolim             #+#    #+#             */
/*   Updated: 2026/04/15 15:21:37 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Step to build path:
1. Direct path command
2. Search path from env_list
3. Is directory?
4. Cmd exist in current directory?
	yes : executable? yes, return command; no, return 126
	no : return 127
 */
char	*build_path(char *cmd, t_minishell *minishell)
{
	char	*full_path;
	char	**path_dir;

	if (ft_strchr(cmd, '/'))
		return (validate_cmd_path(cmd, minishell));
	full_path = NULL;
	path_dir = get_path(minishell->env_list);
	if (path_dir != NULL)
	{
		full_path = search_path(path_dir, cmd);
		free_array_str(path_dir);
	}
	if (full_path)
		return (full_path);
	if (is_directory(cmd))
		return (cmd_is_dir(cmd, minishell));
	if (access(cmd, F_OK) == ACCESS_PERMITTED)
	{
		if (access(cmd, X_OK) == ACCESS_PERMITTED)
			return (cmd);
		if (path_dir == NULL)
			return (report_error_exit_status(minishell, cmd,
					"permission denied", 126));
	}
	return (report_error_exit_status(minishell, cmd, "command not found", 127));
}

/*
envp stands for environment pointer, and it provides the program with access to
system's environment variables, which tells the shell which directories to
search for executable commands.
e.g.
PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
*/

char	*validate_cmd_path(char *cmd, t_minishell *minishell)
{
	if (access(cmd, F_OK) != ACCESS_PERMITTED)
		return (report_error_exit_status(minishell, cmd,
				"no such file or directory", CMD_NOT_FOUND));
	if (access(cmd, X_OK) != ACCESS_PERMITTED)
		return (report_error_exit_status(minishell, cmd,
				"permission denied", CMD_NOT_EXECUTABLE));
	return (cmd);
}

char	**get_path(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->key, "PATH", 5) == 0)
			return (ft_split(current->value, ':'));
		current = current->next;
	}
	return (NULL);
}

/*
!is_directory(full_path)) :
access(..., X_OK) is not enough to confirm “this is an executable program file.”
A directory can pass X_OK (execute/search permission on dirs).
But a directory is not runnable by execve as a command.
Without !is_directory(full_path), search_path() could
wrongly return a directory path as a command match.
*/
char	*search_path(char **path_dir, char *command)
{
	int		i;
	char	*temp_path;
	char	*full_path;

	i = 0;
	while (path_dir[i])
	{
		temp_path = ft_strjoin(path_dir[i], "/");
		full_path = ft_strjoin(temp_path, command);
		free(temp_path);
		if (access(full_path, X_OK) == ACCESS_PERMITTED
			&& !is_directory(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
