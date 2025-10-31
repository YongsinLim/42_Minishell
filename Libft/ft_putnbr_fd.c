/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:37:48 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 11:32:31 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int				place;
	int				digit;
	unsigned int	no_sign_num;

	place = 1;
	digit = 0;
	if (n < 0)
	{
		write(fd, "-", 1);
		n = n * (-1);
	}
	no_sign_num = (unsigned int) n;
	while ((no_sign_num / place) >= 10)
		place = place * 10;
	while (place > 1)
	{
		digit = (no_sign_num / place) + '0';
		write(fd, &digit, 1);
		no_sign_num = no_sign_num % place;
		place = place / 10;
	}
	no_sign_num = no_sign_num + '0';
	write(fd, &no_sign_num, 1);
}

/*
int	main(void)
{
	ft_putnbr_fd(-123464963, 1);
	printf("\n");
	ft_putnbr_fd(-2147483648, 1);
	printf("\n");
	ft_putnbr_fd(2147483647, 1);
	printf("\n");
	ft_putnbr_fd(0, 1);
	printf("\n");
	ft_putnbr_fd(10000, 1);
	return (0);
}
*/