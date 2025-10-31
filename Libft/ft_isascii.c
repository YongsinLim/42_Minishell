/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:28:24 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:30:09 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include "libft.h"

int	ft_isascii(int c)
{
	if (!(c >= 0 && c <= 127))
		return (0);
	return (1);
}

/*
int	main(void)
{
	int	ch;
	ch = 128;

    printf("My Result: %d\n", ft_isascii(ch));
	printf("Ch: %c\n", ch);
	printf("Result of C Library: %d", isascii(ch));
	return (0);
}
*/