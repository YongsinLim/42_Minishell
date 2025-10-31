/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:06:34 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:33:37 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	len;
	char	*str;

	len = 0;
	while (*(s + len) != '\0')
		len++;
	str = (char *) malloc(len + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (*(s + i) != '\0')
	{
		*(str + i) = *(s + i);
		i++;
	}
	*(str + i) = '\0';
	return (str);
}

/*
int main()
{
	char source[] = "GeeksForGeeks";

	char* my_result = ft_strdup(source); 
	char* clib_result = strdup(source); 

	printf("My Result : %s\n", my_result);
	printf("Clib_Result : %s", clib_result);
	return (0);
}
*/