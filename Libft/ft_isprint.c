/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:08:32 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:30:56 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include "libft.h"

int	ft_isprint(int c)
{
	if (!(c >= 32 && c <= 126))
		return (0);
	return (1);
}

/*
int	main(void)
{
	int	ch;
	ch = 57;

    printf("My Result: %d\n", ft_isprint(ch));
	printf("Ch: %c\n", ch);
	printf("Result of C Library: %d", isprint(ch));
    return (0);
}
*/