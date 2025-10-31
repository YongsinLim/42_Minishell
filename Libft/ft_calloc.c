/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:37:46 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 21:26:30 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	unsigned char	*p;
	void			*ptr;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if ((size > 0) && (nmemb > SIZE_MAX / size))
		return (NULL);
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	p = (unsigned char *)ptr;
	while (i < nmemb * size)
	{
		p[i] = 0;
		i++;
	}
	return (ptr);
}

/*
int	main(void)
{
	size_t	num = 5;
	int		*my_arr = (int	*) ft_calloc(num, sizeof(int));

    if (my_arr != NULL)
	{
        printf("Memory allocated successfully using ft_calloc.\n");
        for (size_t i = 0; i < num; i++)
		{
            printf("%d ", my_arr[i]);
        }
        free(my_arr);
    }
	else
        printf("Failed to allocate memory using ft_calloc.\n");

    void *zero_alloc = ft_calloc(0, 10);
    if (zero_alloc == NULL)
	{
        printf("\nResult of Zero-member : %p", ft_calloc(0, 10));
    }

    void *another_zero = ft_calloc(5, 0);
    if (another_zero == NULL)
	{
        printf("\nResult of Zero-size : %p", ft_calloc(5, 0));
    }

    void *overflow_alloc = ft_calloc(SIZE_MAX, 2);
    if (overflow_alloc == NULL)
	{
        printf("\nResult of Overflow : Potential overflow handled correctly.\n");
	}
	return (0);
}
*/