/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:15:52 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:29:43 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>

int	ft_isalnum(int ch)
{
	if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')
		&& !(ch >= '0' && ch <= '9'))
		return (0);
	return (8);
}

/*
int	main(void)
{
	char	ch;
	ch = '5';

    printf("My Result: %d\n", ft_isalnum(ch));
	printf("Ch: %c\n", ch);
	printf("Result of C Library: %d", isalnum(ch));
	return (0);
}
*/
