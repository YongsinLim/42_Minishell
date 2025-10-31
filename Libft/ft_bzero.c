/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:45:39 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:25:30 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*location;

	i = 0;
	location = (char *) s;
	while (i < n)
	{
		location[i] = '\0';
		i++;
	}
}

/*
int main()
{
	char	s[8] = "123456";
	char	my_s[8] = "123456";

	bzero(s + 2, 6);
	printf("Clib_s after bzero: %s\n",s);

	ft_bzero(my_s + 2, 6);
	printf("My_s after ft_bzero: %s\n",my_s);

	return (0);
}
*/