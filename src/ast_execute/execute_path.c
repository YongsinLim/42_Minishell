/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 11:40:26 by yolim             #+#    #+#             */
/*   Updated: 2026/03/01 19:43:24 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(char **cmd_array, t_minishell *minishell)
{
	char	*path;
	char	**envp_array;

	if (!cmd_array || !cmd_array[0])
		return ;
	path = cmd_array[0];
	if (ft_strchr(path, '/') != NULL)
	{
		if (access(path, F_OK) != ACCESS_PERMITTED)
		{
			report_error("no such file or directory", path);
			minishell->last_exit_status = 127;
			return ;
		}
		if (access(path, X_OK) != ACCESS_PERMITTED)
		{
			report_error("permission denied", path);
			minishell->last_exit_status = 126;
			return ;
		}
	}
	else
		path = construct_full_path(minishell->env_list, path);
	if (!path)
	{
		report_error("command not found", cmd_array[0]);
		minishell->last_exit_status = 127;
		return ;
	}
	envp_array = env_list_to_array(minishell->env_list);
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
		if (access(full_path, X_OK) == ACCESS_PERMITTED)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

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
