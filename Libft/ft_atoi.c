/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:09:39 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:24:57 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_whitespace(char input)
{
	return (input == ' ' || input == '\f' || input == '\n' || input == '\r'
		|| input == '\t' || input == '\v');
}

int	ft_atoi(const char *str)
{
	int		i;
	int		output;
	int		sign;

	i = 0;
	output = 0;
	sign = 1;
	while (is_whitespace (*(str + i)))
		i++;
	if (*(str + i) == '-')
	{
		sign = sign * (-1);
		i++;
	}
	else if (*(str + i) == '+')
		i++;
	while (*(str + i) >= '0' && *(str + i) <= '9')
	{
		output = (output * 10) + (*(str + i) - '0');
		i++;
	}
	return (sign * output);
}

/*
int	main(void)
{
	char	*str;

	// str = "		 214 7483648";
	// str = " -214 7483648";
	// str = "  -214 7483648";
	str = " -214748364999";
	// str = "214748364999";
	// str = "02147 483648";
	// str = "	214 7483648";
	// str = "\v214\t7483648";
	// str = "\v214 7483648";
	// str = "\r214 7483648";
	// str = "\f214 7483648";
	// str = "\v214\t7483648";
	// str = " ?214 7483648";
	// str = "- 214 7483648";
	// str = "-214 7483648";
	// str = "-214a7483648";
	// str = "-2147483648";
	// str = "2147483647";
	// str = "--2";
	// str = "--2a -14748364";
	// str = "abcde";
	// str = "1234adsnds";
	// str = "+123456";
	
	printf("Result of ft_atoi = %d\n", ft_atoi(str));
	printf("Result of atoi = %d\n", atoi(str));
	return (0);
}
*/