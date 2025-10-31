/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:01:15 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:29:55 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include "libft.h"

int	ft_isalpha(int ch)
{
	if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z'))
		return (0);
	return (1);
}

/*
int	main(void)
{
	int	ch;
	ch = '\0';

    printf("My Result: %d\n", ft_isalpha(ch));
	printf("Ch: %c\n", ch);
	printf("Result of C Library: %d", isalpha(ch));
	return (0);
}
*/