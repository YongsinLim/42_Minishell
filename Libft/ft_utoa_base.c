/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:54:33 by yolim             #+#    #+#             */
/*   Updated: 2025/09/14 14:02:58 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length(unsigned long long n, int base_output_length)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n = n / base_output_length;
		len++;
	}
	return (len);
}
/* 
base_output posible value = "0123456789abcdef" or "0123456789"
use long long bcos On many systems (especially 64-bit systems where pointers
are 64 bits wide), uintptr_t will be larger than unsigned int (which is
often 32 bits). When you pass the uintptr_t p to ft_utoa_base, its value is
implicitly converted (truncated) to an unsigned int. This means ft_utoa_base
only processes the lower part of the pointer's address, leading to a
shorter output.
*/

char	*ft_utoa_base(unsigned long long num, char *base_output)
{
	int		i;
	int		length;
	char	*str;
	int		base_output_length;

	base_output_length = ft_strlen(base_output);
	length = get_length(num, base_output_length);
	str = (char *) malloc((length + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	i = length - 1;
	while (i >= 0)
	{
		str[i] = base_output[num % base_output_length];
		num = num / base_output_length;
		i--;
	}
	str[length] = '\0';
	return (str);
}
