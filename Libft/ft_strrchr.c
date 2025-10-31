/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:37:50 by yolim             #+#    #+#             */
/*   Updated: 2025/06/03 18:26:39 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int chr)
{
	int	i;
	int	location;

	i = 0;
	location = -1;
	while (str[i] != '\0')
	{
		if ((char) chr == str[i])
			location = i;
		i++;
	}
	if (location >= 0)
		return ((char *) str + location);
	else if ((char) chr == '\0')
		return ((char *) str + i);
	else
		return (0);
}

/*
int	main()
{
	char str[] = "tripouille";
    char chr = 't';

	char* my_ptr = ft_strrchr(str, chr + 256);
    char* clib_ptr = strrchr(str, chr + 256);


	printf("Last occurrence of %c in %s is at %s\n", chr, str, my_ptr);
	printf("Last occurrence of %c in %s is at %s", chr, str, clib_ptr);
	return(0);
}
*/