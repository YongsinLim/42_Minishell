/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:10:42 by yolim             #+#    #+#             */
/*   Updated: 2025/05/29 15:28:58 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (0);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

/*
int	main(void)
{
	t_list	*my_shopping_list_head = NULL;

	t_list *item_eggs = ft_lstnew("Eggs");
	t_list *item_bread = ft_lstnew("Bread");
	t_list *item_milk = ft_lstnew("Milk");

	ft_lstadd_front(&my_shopping_list_head, item_bread);
	ft_lstadd_front(&my_shopping_list_head, item_eggs);
	ft_lstadd_front(&my_shopping_list_head, item_milk);

	printf("%s\n", (char *)ft_lstlast(my_shopping_list_head)->content);
	return (0);
}
*/