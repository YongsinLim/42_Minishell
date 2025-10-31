/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:53:59 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:30:23 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include "libft.h"

int	ft_isdigit(int nbr)
{
	if (!(nbr >= '0' && nbr <= '9'))
		return (0);
	return (1);
}

/*
int	main(void)
{
	int	num;
	num = 'a';

    printf("My Result: %d\n", ft_isdigit(num));
	printf("num: %c\n", num);
	printf("Result of C Library: %d", isdigit(num));
	return (0);
}
*/