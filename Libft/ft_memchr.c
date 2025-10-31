/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:50:57 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:31:37 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	p = (unsigned char *) ptr;
	i = 0;
	while (i < n)
	{
		if (*(p + i) == (unsigned char) c)
			return ((void *)(p + i));
		i++;
	}
	return (0);
}

/*
int main()
{
	char	myStr[] = "Hello World!";
	char	*clib_Ptr = (char*)memchr(myStr, 'l', 12);
	char	*my_Ptr = (char*)ft_memchr(myStr, 'l', 12);

	printf("Result of memchr: %s\n", clib_Ptr);
	printf("Result of ft_memchr: %s", my_Ptr);
	return(0);
}
*/