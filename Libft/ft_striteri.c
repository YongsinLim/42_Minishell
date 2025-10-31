/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:16:00 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:33:41 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (s == NULL || f == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/*
static void	function(unsigned int index, char *c)
{
	unsigned int	i;

	i = 0;
	if (index % 2 == 0)
	{
		if (c[i] >= 'a' && c[i] <= 'z')
			c[i] = c[i] - 32;
	}
	else
		c[i] = c[i] + 1;
}

int	main(void)
{
	char	str[] = "hello world";

	ft_striteri(str, function);
	printf("Result : %s\n", str);

	char *null_str = NULL;

	ft_striteri(null_str, function);
	printf("Result of Null_Str: %s\n", null_str);
	
	char str1[] = "hello world";

	ft_striteri(str1, NULL);
	printf("Result of Null_function: %s\n", str1);
	return (0);
}
*/