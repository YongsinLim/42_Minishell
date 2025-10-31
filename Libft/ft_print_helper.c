/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:29:58 by yolim             #+#    #+#             */
/*   Updated: 2025/09/14 14:02:38 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* ft_ptint_char : putchar & return 1 count */
int	ft_putchar_getcount(char character)
{
	int		count;

	write(1, &character, 1);
	count = 1;
	return (count);
}

int	ft_putstr_getcount(char *str)
{
	int	count;

	if (str == NULL)
		str = "(null)";
	ft_putstr_fd(str, 1);
	count = ft_strlen(str);
	return (count);
}

int	ft_putint_getcount(int num)
{
	int		count;
	char	*str;

	str = ft_itoa_base(num, "0123456789");
	if (str)
	{
		ft_putstr_fd(str, 1);
		count = ft_strlen(str);
		free(str);
		return (count);
	}
	else
		return (-1);
}

int	ft_put_unbr_getcount(unsigned int num, char flag)
{
	int		count;
	char	*str;

	str = NULL;
	if (flag == 'u')
		str = ft_utoa_base(num, "0123456789");
	else if (flag == 'x')
		str = ft_utoa_base(num, "0123456789abcdef");
	else if (flag == 'X')
		str = ft_utoa_base(num, "0123456789ABCDEF");
	if (str)
	{
		ft_putstr_fd(str, 1);
		count = ft_strlen(str);
		free(str);
		return (count);
	}
	else
		return (-1);
}

//uintptr_t is used to be large enough to hold any valid void* pointer value.
int	ft_putptr_getcount(uintptr_t p)
{
	int		count;
	char	*str;

	if (p == 0)
		count = ft_putstr_getcount("(nil)");
	else
	{
		write(1, "0x", 2);
		count = 2;
		str = ft_utoa_base(p, "0123456789abcdef");
		if (str)
		{
			ft_putstr_fd(str, 1);
			count = count + ft_strlen(str);
			free(str);
		}
		else
			return (-1);
	}
	return (count);
}
