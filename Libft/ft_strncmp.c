/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:56:41 by yolim             #+#    #+#             */
/*   Updated: 2025/05/30 17:06:14 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t count)
{
	size_t	i;

	i = 0;
	if (count == 0)
		return (0);
	while ((i < count) && (*(str1 + i) != '\0' || *(str2 + i) != '\0'))
	{
		if (*(str1 + i) != *(str2 + i))
			return ((unsigned char) str1[i] - (unsigned char) str2[i]);
		i++;
	}
	return (0);
}

/*
int	main(void)
{
	char	*str1;
	char	*str2;
	size_t	count;
	int		my_result;
	int		clib_result;

	str1 = "\200";
	str2 = "\0";
	count = 1;
	my_result = ft_strncmp(str1, str2, count);
	printf("My Result = %d\n", my_result);
	clib_result = strncmp(str1, str2, count);
	printf("C Library Result = %d", clib_result);
	return(0);
}
*/