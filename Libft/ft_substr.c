/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:12 by yolim             #+#    #+#             */
/*   Updated: 2025/06/03 18:47:57 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_length;
	char	*substr;

	if (s == NULL)
		return (ft_strdup(""));
	s_length = ft_strlen(s);
	if (start >= s_length)
		return (ft_strdup(""));
	if (len > s_length - start)
		len = s_length - start;
	substr = (char *) ft_calloc((len + 1), sizeof(char));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while ((i < len) && (*(s + start + i) != '\0'))
	{
		*(substr + i) = *(s + start + i);
		i++;
	}
	return (substr);
}

/*
int main()
{
	char s[] = "Hello, Geeks!";
	char s1[] = "";

	printf("My_Result : %s\n", ft_substr(s, 12, 5));
	printf("My_Result : %s\n", ft_substr(s, 120, 5));
	printf("My_Result : %s\n", ft_substr(s1, 0, 5));
	printf("My_Result : %s\n", ft_substr(s, 7, 5));
	printf("My_Result : %s\n", ft_substr(s + 7, 0, 5));
	return (0);
}
*/