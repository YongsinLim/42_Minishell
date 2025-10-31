/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 21:59:48 by yolim             #+#    #+#             */
/*   Updated: 2025/06/01 13:24:42 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	i;

	if (s == NULL || f == NULL)
		return (0);
	str = (char *) malloc((ft_strlen(s) + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s))
	{
		str[i] = (*f)(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
static char	function(unsigned int index, char c)
{
	if (index % 2 == 0)
	{
		if (c >= 'a' && c <= 'z')
			return (c - 32);
		else
			return (c);
	}
	else
		return (c + 1);
}

int	main(void)
{
	printf("Result : %s\n", ft_strmapi("abc", function));
	printf("Result : %s\n", ft_strmapi("123abc|@#", function));
	printf("Result : %s\n", ft_strmapi(NULL, function));
	printf("Result : %s\n", ft_strmapi("abc", NULL));
	return (0);
}
*/