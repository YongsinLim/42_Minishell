/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:54:19 by yolim             #+#    #+#             */
/*   Updated: 2025/05/30 16:30:55 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *source, size_t n)
{
	size_t				i;
	unsigned char		*dst;
	const unsigned char	*src;

	dst = (unsigned char *) dest;
	src = (const unsigned char *) source;
	if (dst == src)
		return (dst);
	i = 0;
	while (i < n)
	{
		*(dst + i) = *(src + i);
		i++;
	}
	return ((void *)(dst));
}

/*
int main()
{
	char	src[] = "Hello";
	char	dest[7] = "123456";

	char	my_src[] = "Hello";
	char	my_dest[7] = "123456";

	memcpy(dest + 1, src, 3);
	printf("dest after memcpy: %s\n",dest);

	ft_memcpy(my_dest + 1, my_src, 3);
	printf("my_dest after ft_memcpy: %s", my_dest);

	return (0);
}
*/