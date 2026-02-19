/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 11:40:26 by yolim             #+#    #+#             */
/*   Updated: 2026/02/19 22:59:38 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// In execute_path.c

void execute(char **cmd_array, t_env **env_list)
{
    char *path;
    char **envp_array;

    if (!cmd_array || !cmd_array[0])
        report_error("command not found", NULL, NULL, 127);
    envp_array = env_list_to_array(*env_list);
    if (!envp_array)
        error_exit("Malloc failed for envp_array");
    path = cmd_array[0];
    if (ft_strchr(path, '/') != NULL)
    {
        if (access(path, F_OK) != ACCESS_PERMITTED)
        {
            free_array_str(envp_array);
            report_error("no such file or directory", NULL, cmd_array, 127);
        }
        if (access(path, X_OK) != ACCESS_PERMITTED)
        {
            free_array_str(envp_array);
            report_error("permission denied", NULL, cmd_array, 126);
        }
    }
    else
        path = construct_full_path(envp_array, path); // Use the converted array
    if (!path)
    {
        free_array_str(envp_array);
        report_error("command not found", cmd_array[0], cmd_array, 127);
    }
    if (execve(path, cmd_array, envp_array) == ERROR)
    {
        perror(path);
        free_array_str(cmd_array);
        free_array_str(envp_array); // Make sure to free on failure!
        if (path != cmd_array[0])
            free(path);
        exit(126);
    }
}

/*
envp stands for environment pointer, and it provides the program with access to
the system's environment variables, which tells the shell which directories to
search for executable commands.
e.g.
PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
*/

char	*construct_full_path(char *envp[], char *command)
{
	char	**path_dir;
	char	*full_path;

	path_dir = get_path(envp);
	if (!path_dir)
		return (NULL);
	full_path = search_path(path_dir, command);
	free_array_str(path_dir);
	return (full_path);
}

char	**get_path(char *envp[])
{
	char	**path;

	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			break ;
		envp++;
	}
	if (!*envp)
		return (NULL);
	path = ft_split(*envp + 5, ':');
	return (path);
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

void	report_error(char *msg, char *param, char **free_me, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	if (param)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(param, 2);
	}
	ft_putstr_fd("\n", 2);
	if (free_me)
		free_array_str(free_me);
	if (exit_code > 0)
		exit(exit_code);
}
