/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:32:56 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:34:11 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (*(str + i) != '\0')
	{
		i++;
	}
	return (i);
}

/*
int	main(void)
{
	char str[] = " ";
    int	my_count = ft_strlen(str);
	printf("My Length of string is = %d\n", my_count);


	int clib_count = strlen(str);
	printf("C Library's Length of string is = %d", clib_count);

    return (0);
}
*/