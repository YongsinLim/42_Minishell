/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:10:31 by yolim             #+#    #+#             */
/*   Updated: 2026/04/14 20:07:12 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	skip_ws_and_sign(char *str, int *i)
{
	int	sign;

	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static int	parse_exit_code(char *str, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	res;
	unsigned long long	limit;

	i = 0;
	sign = skip_ws_and_sign(str, &i);
	if (!ft_isdigit(str[i]))
		return (FALSE);
	res = 0;
	limit = (unsigned long long)LLONG_MAX + (sign == -1);
	while (ft_isdigit(str[i]))
	{
		if (res > limit / 10
			|| (res == limit / 10 && (unsigned long long)(str[i] - '0')
			> limit % 10))
			return (FALSE);
		res = res * 10 + (str[i++] - '0');
	}
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	*out = (long long)(res * sign);
	return (str[i] == '\0');
}

int	ft_exit(char **argv, t_minishell *minishell)
{
	int			interactive;
	long long	code;

	interactive = isatty(STDIN_FILENO);
	if (interactive)
		ft_putstr_fd("exit\n", 2);
	if (argv[1] && !parse_exit_code(argv[1], &code))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_and_exit(minishell, SYNTAX_ERROR);
	}
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		minishell->last_exit_status = SHELL_FAILURE;
		return (SHELL_FAILURE);
	}
	if (!argv[1])
		cleanup_and_exit(minishell, minishell->last_exit_status);
	return (cleanup_and_exit(minishell, (unsigned char)code), SHELL_SUCCESS);
}

/*
exit code % 256 bcos :
operating system truncates any exit code you provide to exit() to its least
significant 8 bits.
       * 0 typically indicates success.
       * Any value from 1 to 255 typically indicates an error or a specific
	     status.
*/

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
