/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:40:55 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/11 17:40:55 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(char **argv)
{
	int	i;
	int	do_newline;

	i = 1;
	do_newline = 1;
	//check for n
	while (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
	{
		do_newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (do_newline)
		ft_putstr_fd("\n", 1);
	return (0);
}