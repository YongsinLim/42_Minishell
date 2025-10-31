/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:23:38 by yolim             #+#    #+#             */
/*   Updated: 2025/07/12 12:47:02 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
#include <bsd/string.h>
*/

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	while ((i + 1 < size) && (*(src + i) != '\0'))
	{
		*(dest + i) = *(src + i);
		i++;
	}
	if (size != 0)
		dest[i] = '\0';
	while (*(src + i) != '\0')
	{
		i++;
	}
	return (i);
}

/*
int	main(void)
{
	const char	*src;
    const char	*src2;
	char	dest[8];
    char	dest2[8];

	int my_count;
    int clib_count;

	src = "HeHeHeHeHe";
    src2 = "HeHeHeHeHe";
	my_count = ft_strlcpy(dest, src, 8);
	clib_count = strlcpy(dest2, src2, 8);

	printf("Result: %s\n", dest);
	printf("Result of my count: %d\n", my_count);
    printf("Result: %s\n", dest2);
	printf("Result of C Library Count: %d\n", clib_count);
	return(0);
}
*/