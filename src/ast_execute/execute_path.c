/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 11:40:26 by yolim             #+#    #+#             */
/*   Updated: 2026/04/12 19:52:30 by yolim            ###   ########.fr       */
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
	if (!envp_array)
	{
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

char	*full_path_cmd(char *cmd, t_minishell *minishell)
{
	if (access(cmd, F_OK) != ACCESS_PERMITTED)
		return (exit_status(minishell, cmd,
				"no such file or directory", 127));
	if (access(cmd, X_OK) != ACCESS_PERMITTED)
		return (exit_status(minishell, cmd,
				"permission denied", 126));
	return (cmd);
}

char	*cmd_with_dir(char *cmd, t_minishell *minishell)
{
	// Special case: . (source builtin) without argument should exit with 2
	if (ft_strncmp(cmd, ".", 2) == 0)
		return (exit_status(minishell, cmd, "filename argument"
				"required", 127));
	// Special case: .. (parent directory reference) should treated as not found
	if (ft_strncmp(cmd, "..", 3) == 0)
		return (exit_status(minishell, cmd, "command not found",
				127));
	// directory case
	return (exit_status(minishell, cmd, "Is a directory", 126));
}

char	*build_path(char *cmd, t_minishell *minishell)
{
	char	*full_path;
	char	**path_dir;

	if (ft_strchr(cmd, '/'))
		return (full_path_cmd(cmd, minishell));
	full_path = NULL;
	path_dir = get_path(minishell->env_list);
	if (path_dir != NULL)
	{
		full_path = search_path(path_dir, cmd);
		free_array_str(path_dir);
	}
	if (full_path)
		return (full_path);
	if (is_directory_path(cmd))
		return (cmd_with_dir(cmd, minishell));
	if (access(cmd, F_OK) == ACCESS_PERMITTED)
	{
		if (access(cmd, X_OK) == ACCESS_PERMITTED)
			return (cmd); // Command exists and is executable in current dir
		// PATH doesn't exist, and it is not a executable return 126
		if (path_dir == NULL)
			return (exit_status(minishell, cmd, "permission denied", 126));
	}
	return (exit_status(minishell, cmd, "command not found", 127));
}

// todo remove
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
	DIR	*dir;

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
