/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:40:52 by yolim             #+#    #+#             */
/*   Updated: 2025/06/03 17:05:44 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int ch)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((char) ch == str[i])
			return ((char *) str + i);
		i++;
	}
	if ((char) ch == '\0')
		return ((char *) str + i);
	return (0);
}

/*
int main()
{
    const char* str = "tripouille";
    char    ch = 't';

    const char* my_result = ft_strchr(str, ch + 256);
    const char* clib_result = strchr(str, ch + 256);

    printf("Character '%c' found at position: %s\n", ch, my_result);
    printf("Character '%c' found at position: %s\n", ch, clib_result);
    return (0);
}
*/