/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:00:13 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:35:02 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include "libft.h"

int	ft_tolower(int ch)
{
	if (ch >= 65 && ch <= 90)
		ch = ch + 32;
	return (ch);
}

/*
int	main(void)
{
	char	ch;
	ch = 'A';

    printf("My Result: %c\n", ft_tolower(ch));
	printf("Ch: %c\n", ch);
	printf("Result of C Library: %c", tolower(ch));
	return (0);
}
*/