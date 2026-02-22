/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 17:39:24 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/22 17:39:24 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char **argv, t_env **env)
{
	char	*path;
	char	cwd[1024];
	char	new_cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		cwd[0] = '\0'; // Failsafe
	if (!argv[1]) // No argument: go to HOME
	{
		path = get_env_val(*env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (ft_strncmp(argv[1], "-", 2) == 0) // "-": go to OLDPWD
	{
		path = get_env_val(*env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (1);
		}
	}
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	if (cwd[0] != '\0')
		update_env(env, "OLDPWD", cwd);
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		update_env(env, "PWD", new_cwd);
	if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
		ft_putendl_fd(new_cwd, 1);

	return (0);
}
