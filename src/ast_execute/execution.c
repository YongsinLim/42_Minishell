/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:00:48 by yolim             #+#    #+#             */
/*   Updated: 2026/04/15 17:50:54 by yolim            ###   ########.fr       */
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
		return ((void)report_error_exit_status(minishell, cmd_name,
				"command not found", CMD_NOT_FOUND));
	path = build_path(cmd_array[0], minishell);
	if (!path)
		return ;
	envp_array = prepare_execute_env(path, cmd_array, minishell);
	if (!envp_array)
		return ;
	if (execve(path, cmd_array, envp_array) == ERROR)
	{
		perror(path);
		minishell->last_exit_status = CMD_NOT_EXECUTABLE;
		free_array_str(envp_array);
		if (path != cmd_array[0])
			free(path);
	}
}

/*
update_env("_", path, minishell)
updates the environment variable _ to the command path you’re about to run.
*/
char	**prepare_execute_env(char *path, char **cmd_array,
	t_minishell *minishell)
{
	char	**envp_array;

	update_env("_", path, minishell);
	envp_array = env_list_to_array(minishell->env_list);
	if (!envp_array)
	{
		report_error("calloc error", "env_list_to_array");
		minishell->last_exit_status = SHELL_FAILURE;
		if (path != cmd_array[0])
			free(path);
		return (NULL);
	}
	return (envp_array);
}

char	**env_list_to_array(t_env *env_list)
{
	int		count;
	char	**array;
	int		i;
	char	*temp;

	count = count_env_nodes(env_list);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (env_list)
	{
		if (env_list->value != NULL)
		{
			temp = ft_strjoin(env_list->key, "=");
			array[i] = ft_strjoin(temp, env_list->value);
			free(temp);
			i++;
		}
		env_list = env_list->next;
	}
	array[i] = NULL;
	return (array);
}
