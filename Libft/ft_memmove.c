/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:44:58 by yolim             #+#    #+#             */
/*   Updated: 2025/05/31 20:39:47 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *source, size_t n)
{
	size_t				i;
	unsigned char		*dst;
	const unsigned char	*src;

	if ((n == 0) || (dest == source))
		return (dest);
	dst = (unsigned char *) dest;
	src = (const unsigned char *) source;
	i = 0;
	if (dst < src)
	{
		while (i < n)
		{
			*(dst + i) = *(src + i);
			i++;
		}
	}
	else
	{
		i = n;
		while (i--)
			*(dst + i) = *(src + i);
	}
	return (dest);
}

/*
int main()
{
	char	dest[7] = "123456";
	char	my_dest[7] = "123456";
	char	my_dest1[7] = "abcdef";

	memmove(dest + 1, dest, 3);
	printf("dest with overlap after memmove: %s\n",dest);
	ft_memmove(my_dest + 1, my_dest, 3);
	printf("my_dest with overlap after ft_memmove: %s\n", my_dest);

	memmove(my_dest1, my_dest1, 3);
	printf("my_dest1 with overlap after memmove: %s\n",my_dest1);
	ft_memmove(my_dest1, my_dest1, 3);
	printf("my_dest1 with overlap after ft_memmove: %s\n",my_dest1);
	return (0);
}
*/