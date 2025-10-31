/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:24:07 by yolim             #+#    #+#             */
/*   Updated: 2025/07/12 12:47:18 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
#include <bsd/string.h>
*/
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	x;
	size_t	count_dst;
	size_t	count_src;
	size_t	remaining_src;

	count_dst = ft_strlen(dst);
	count_src = ft_strlen((char *)src);
	if ((size <= count_dst) || (size == 0))
		return (size + count_src);
	x = 0;
	remaining_src = size - count_dst - 1;
	i = count_dst;
	while ((*(src + x) != '\0') && (x < remaining_src))
	{
		*(dst + i) = *(src + x);
		i++;
		x++;
	}
	*(dst + i) = '\0';
	return (count_dst + count_src);
}

/*
int	main(void)
{
	char	dst[50] = "Hello";
	char	dst2[50] = "Hello";

	char	*src = "123456";
	size_t	size = 4;
	size_t	my_result = ft_strlcat(dst, src, size);
	size_t	clib_result = strlcat(dst2, src, size);

	printf("Result from ft_strlcat: %ld\n", my_result);
	printf("Result dst: %s\n", dst);
	printf("Result from strlcat: %ld\n", clib_result);
	printf("Result dst: %s\n", dst);
	printf("Result from ft_strlcat: %ld\n", ft_strlcat("", "toto", 0));
	printf("Result from strlcat: %ld\n", strlcat("", "toto", 0));
	return(0);
}
*/