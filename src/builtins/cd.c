/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 17:39:24 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/03 00:44:41 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_target_path(char **argv, t_minishell *minishell)
{
	char	*path;

	if (!argv[1])
	{
		path = get_var_value("HOME", minishell);
		if (ft_strncmp(path, "", 1) == 0)
			report_error("cd", "HOME not set");
		return (path);
	}
	if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = get_var_value("OLDPWD", minishell);
		if (!path)
			report_error("cd", "OLDPWD not set");
		return (path);
	}
	return (argv[1]);
}

/*
cd [no arg] = go to home
cd ~ = go to home
cd - = go to OLDPWD
 */

void	update_env(char *key, char *value, t_minishell *minishell)
{
	t_env	*current;
	t_env	*last;
	t_env	*new;

	current = minishell->env_list;
	last = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		last = current;
		current = current->next;
	}
	// Key Not found - create new node
	new = new_env_node(key, value);
	if (!new)
		return ;
	if (last == NULL)
		minishell->env_list = new;
	else
		last->next = new;
}

int	ft_cd(char **argv, t_minishell *minishell)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	new_cwd[PATH_MAX];

	if (argv[1] && argv[2])
		return (report_error("cd", "too many arguments"), SHELL_FAILURE);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		cwd[0] = '\0';
	path = get_target_path(argv, minishell);
	if (!path)
		return (SHELL_FAILURE);
	if (chdir(path) == ERROR)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		printf("%s: %s\n", strerror(2), path);
		return (SHELL_FAILURE);
	}
	update_env("OLDPWD", cwd, minishell);
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		update_env("PWD", new_cwd, minishell);
	if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
		ft_putendl_fd(new_cwd, 1);
	return (SHELL_SUCCESS);
}
