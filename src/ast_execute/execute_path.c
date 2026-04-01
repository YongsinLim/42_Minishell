/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 11:40:26 by yolim             #+#    #+#             */
/*   Updated: 2026/03/31 22:55:21 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(char **cmd_array, t_minishell *minishell)
{
	char	*cmd_name;
	char	*path;
	char	**envp_array;

	cmd_name = "";
	if (cmd_array && cmd_array[0])
		cmd_name = cmd_array[0];
	if (!cmd_array || !cmd_array[0] || cmd_array[0][0] == '\0')
	{
		report_error(cmd_name, "command not found");
		minishell->last_exit_status = 127;
		return ;
	}
	path = build_path(cmd_array[0], minishell);
	if (!path)
		return ;
	update_env("_", path, minishell);
	envp_array = env_list_to_array(minishell->env_list);
	if (!envp_array) {
		report_error("malloc error", "enve_list_to_array");
		minishell->last_exit_status = SHELL_FAILURE;
		if (path != cmd_array[0])
			free(path);
		return ;
	}
	if (execve(path, cmd_array, envp_array) == ERROR)
	{
		perror(path);
		minishell->last_exit_status = 126;
		free_array_str(envp_array);
		if (path != cmd_array[0])
			free(path);
	}
}

/*
envp stands for environment pointer, and it provides the program with access to
the system's environment variables, which tells the shell which directories to
search for executable commands.
e.g.
PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
*/

char	*build_path(char *cmd, t_minishell *minishell)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != ACCESS_PERMITTED)
		{
			report_error(cmd, "no such file or directory");
			minishell->last_exit_status = 127;
			return (NULL);
		}
		if (access(cmd, X_OK) != ACCESS_PERMITTED)
		{
			report_error(cmd, "permission denied");
			minishell->last_exit_status = 126;
			return (NULL);
		}
		return (cmd);
	}
	path = construct_full_path(minishell->env_list, cmd);
	if (!path)
	{
		report_error(cmd, "command not found");
		minishell->last_exit_status = 127;
	}
	return (path);
}

char	*construct_full_path(t_env *env_list, char *command)
{
	char	**path_dir;
	char	*full_path;

	path_dir = get_path(env_list);
	if (!path_dir)
		return (NULL);
	full_path = search_path(path_dir, command);
	free_array_str(path_dir);
	return (full_path);
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

int	is_directory_path(char *path)
{
	DIR *dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (TRUE);
	}
	return (FALSE);
}

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
			&& !is_directory_path(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
