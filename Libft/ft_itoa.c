/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:32:45 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:31:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length(long n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		i;
	int		length;
	long	num;
	char	*str;

	num = n;
	length = get_length(num);
	str = (char *) malloc((length + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (num < 0)
		num = -num;
	i = length - 1;
	while (i >= 0)
	{
		str[i] = (num % 10) + '0';
		num = num / 10;
		i--;
	}
	if (n < 0)
		str[0] = '-';
	str[length] = '\0';
	return (str);
}

/*
int	main(void)
{
	printf("Result of ft_itoa: %s\n", ft_itoa(-2147483648));
	printf("Result of ft_itoa: %s\n", ft_itoa(-20000));
	printf("Result of ft_itoa: %s\n", ft_itoa(0));
	printf("Result of ft_itoa: %s\n", ft_itoa(2147483647));
	printf("Result of ft_itoa: %s\n", ft_itoa('\0'));
	return (0);
}
*/