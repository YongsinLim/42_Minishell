/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:10:31 by yolim             #+#    #+#             */
/*   Updated: 2026/03/27 17:39:47 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (FALSE);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (FALSE);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	ft_exit(char **argv, t_minishell *minishell)
{
	ft_putstr_fd("exit\n", 2);
	if (argv[1] && !ft_is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_and_exit(minishell, 255);
	}
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		minishell->last_exit_status = 1;
		return (minishell->last_exit_status);
	}
	if (!argv[1])
		cleanup_and_exit(minishell, minishell->last_exit_status);
	cleanup_and_exit(minishell, ft_atoi(argv[1]) % 256);
	return (0);
}

/*
exit code % 256 bcos :
operating system truncates any exit code you provide to exit() to its least
significant 8 bits.
       * 0 typically indicates success.
       * Any value from 1 to 255 typically indicates an error or a specific
	     status.
*/
