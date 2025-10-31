/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:13:26 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:32:07 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	p = (unsigned char *) ptr;
	i = 0;
	while (i < n)
	{
		*(p + i) = (unsigned char) c;
		i++;
	}
	return (p);
}

/*
int main()
{
	char str[50] = "GeeksForGeeks is for programming geeks.";
	char str1[50] = "GeeksForGeeks is for programming geeks.";

	printf("Before running function: %s\n", str1);
	ft_memset(str1 + 13, '.', 3);
	printf("After ft_memset(): %s\n", str1);

	memset(str + 13, '.', 3);
	printf("After memset(): %s\n", str);

	return (0);
}
*/