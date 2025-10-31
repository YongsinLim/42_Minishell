/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:45:52 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:35:11 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include "libft.h"

int	ft_toupper(int ch)
{
	if (ch >= 97 && ch <= 122)
		ch = ch - 32;
	return (ch);
}

/*
int	main(void)
{
	char	ch;
	ch = 'a';

    printf("My Result: %c\n", ft_toupper(ch));
	printf("Ch: %c\n", ch);
	printf("Result of C Library: %c", toupper(ch));
	return (0);
}
*/