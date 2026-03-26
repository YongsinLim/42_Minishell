/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 18:46:25 by jenlee            #+#    #+#             */
/*   Updated: 2026/03/25 16:30:20 by yolim            ###   ########.fr       */
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
		printf("%s: %s\n", path, strerror(errno));
		return (SHELL_FAILURE);
	}
	update_env("OLDPWD", cwd, minishell);
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		update_env("PWD", new_cwd, minishell);
	if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
		ft_putendl_fd(new_cwd, 1);
	return (SHELL_SUCCESS);
}
