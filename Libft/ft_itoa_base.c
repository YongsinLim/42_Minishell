/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:55:10 by yolim             #+#    #+#             */
/*   Updated: 2025/09/14 14:05:24 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length(long n, int base_output_length)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / base_output_length;
		len++;
	}
	return (len);
}

/* base_output posible value = "0123456789abcdef" or "0123456789" */
char	*ft_itoa_base(int n, char *base_output)
{
	int		i;
	int		length;
	long	num;
	char	*str;
	int		base_output_length;

	num = n;
	base_output_length = ft_strlen(base_output);
	length = get_length(n, base_output_length);
	str = (char *) malloc((length + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (num < 0)
		num = -num;
	i = length - 1;
	while (i >= 0)
	{
		str[i] = base_output[num % base_output_length];
		num = num / base_output_length;
		i--;
	}
	if (n < 0)
		str[0] = '-';
	str[length] = '\0';
	return (str);
}
