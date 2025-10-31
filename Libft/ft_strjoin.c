/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:50:19 by yolim             #+#    #+#             */
/*   Updated: 2025/05/31 15:27:16 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t			i;
	size_t			s1_len;
	size_t			s2_len;
	char			*ptr;

	if ((s1 == NULL) || (s2 == NULL))
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ptr = (char *) malloc((s1_len + s2_len + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < s1_len)
	{
		ptr[i] = s1[i];
		i++;
	}
	while (i - s1_len < s2_len)
	{
		ptr[i] = s2[i - s1_len];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

/*
int	main(void)
{
	char	*string1 = "Hello ";
	char	*string2 = "World";

	printf("Result of strjoin : %s", ft_strjoin(string1, string2));
	return (0);
}
*/