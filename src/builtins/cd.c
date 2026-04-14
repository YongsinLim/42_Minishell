/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 18:46:25 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 15:05:17 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
cd [no arg] = go to home
cd -- = go to home
cd ~ = go to home
cd - = go to OLDPWD
*/
int	ft_cd(char **argv, t_minishell *minishell)
{
	char	cwd[PATH_MAX];
	char	*path;
	char	new_cwd[PATH_MAX];
	int		got_new_cwd;

	if (argv[1] && argv[2])
		return (report_error("cd", "too many arguments"), SHELL_FAILURE);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		cwd[0] = '\0';
	path = get_target_path(argv, minishell);
	if (!path)
		return (SHELL_FAILURE);
	if (chdir(path) == ERROR)
		return (report_chdir_error(path), free(path), SHELL_FAILURE);
	free(path);
	update_env("OLDPWD", cwd, minishell);
	got_new_cwd = (getcwd(new_cwd, sizeof(new_cwd)) != NULL);
	if (got_new_cwd)
		update_env("PWD", new_cwd, minishell);
	if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0 && got_new_cwd)
		ft_putendl_fd(new_cwd, 1);
	return (SHELL_SUCCESS);
}

char	*get_target_path(char **argv, t_minishell *minishell)
{
	char	*path;

	if (!argv[1] || ft_strncmp(argv[1], "--", 3) == 0)
	{
		path = get_var_value("HOME", minishell);
		if (!path || path[0] == '\0')
		{
			free(path);
			report_error("cd", "HOME not set");
			return (NULL);
		}
		return (path);
	}
	if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = get_var_value("OLDPWD", minishell);
		if (!path || path[0] == '\0')
		{
			free(path);
			report_error("cd", "OLDPWD not set");
			return (NULL);
		}
		return (path);
	}
	return (ft_strdup(argv[1]));
}
