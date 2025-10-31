/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:35:29 by yolim             #+#    #+#             */
/*   Updated: 2025/07/12 12:50:35 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include <bsd/string.h>
*/
#include "libft.h"

static int	is_correct(const char *big, const char *little, size_t len)
{
	size_t	x;

	x = 0;
	while ((*(little + x) != '\0') && (x < len))
	{
		if (*(big + x) != *(little + x))
		{
			return (0);
		}
		x++;
	}
	if (*(little + x) != '\0')
		return (0);
	return (1);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (*little == '\0')
		return ((char *) big);
	if (len == 0)
		return (0);
	i = 0;
	while ((*(big + i) != '\0') && (i < len))
	{
		if (*(big + i) == *little)
		{
			if ((is_correct(big + i, little, len - i) == 1))
				return ((char *)(big + i));
		}
		i++;
	}
	return (NULL);
}

/*
int	main(void)
{
	const char	*big;
	const char	*little;
	char		*result;
	char		*result2;
	const char *big_ptr;
	const size_t len = 5 * sizeof(char);

	big = "Foo Bar Baz";
	little = "Bar";
	big_ptr = big + 1;

	result = ft_strnstr(big_ptr, little, len);
	printf("Result of ft_strnstr: %s\n", result);
	result2 = strnstr(big_ptr, little, len);
	printf("Result of strnstr: %s\n", result2);
	return (0);
}
*/