/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:47:29 by yolim             #+#    #+#             */
/*   Updated: 2025/06/03 17:42:02 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	is_set(char const *set, char input)
{
	size_t	i;

	i = 0;
	if (*(set + i) == '\0')
		return (0);
	i = 0;
	while (*(set + i) != '\0')
	{
		if (input == *(set + i))
			return (1);
		i++;
	}
	return (0);
}

static void	assign(char *str, char const *s1, size_t start, size_t end)
{
	size_t	i;

	i = 0;
	while (start < end)
	{
		*(str + i) = *(s1 + start);
		i++;
		start++;
	}
	*(str + i) = '\0';
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	if (s1 == NULL)
		return (0);
	if (*s1 == '\0')
		return (ft_strdup(""));
	start = 0;
	while (is_set(set, *(s1 + start)) == 1)
		start++;
	end = ft_strlen(s1);
	while ((end > start) && (is_set(set, s1[end - 1]) == 1))
		end--;
	str = (char *) malloc((end - start + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	assign(str, s1, start, end);
	return (str);
}

/*
int	main(void)
{
	printf("Actual : %s, Expected : %s\n", ft_strtrim("abcba", "ab"), "c");
	printf("Actual : %s, Expected : %s\n", ft_strtrim(" abcba ", " "), "abcba");
	printf("Actual : %s, Expected : %s\n", ft_strtrim(" ab ba ", " "), "ab ba");
	printf("Actual : %s, Expected : %s\n", ft_strtrim("abba", " \n\t"), "abba");
	printf("Actual : %s, Expected : %s\n", ft_strtrim("abba", ""), "abba");
	printf("Actual : %s, Expected : %s\n", ft_strtrim(NULL, "123"), "null");
	printf("Actual : %s, Expected : %s\n", ft_strtrim("", ""), "");
}
*/