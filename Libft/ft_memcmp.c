/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:14:00 by yolim             #+#    #+#             */
/*   Updated: 2025/05/30 16:04:26 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*c1;
	unsigned char	*c2;

	c1 = (unsigned char *) s1;
	c2 = (unsigned char *) s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n)
	{
		if (*(c1 + i) != *(c2 + i))
			return (*(c1 + i) - *(c2 + i));
		i++;
	}
	return (0);
}

/*
int main() {
	char s1[10] = "-91";
	char s2[10] = "-92";

	printf("Result of memcmp: %d\n", memcmp(s1, s2, 3));
	printf("Result of ft_memcmp: %d\n", ft_memcmp(s1, s2, 3));

	return (0);
}
*/