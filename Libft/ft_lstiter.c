/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:59:04 by yolim             #+#    #+#             */
/*   Updated: 2025/05/30 13:40:31 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL || f == NULL)
		return ;
	while (lst != NULL)
	{
		f(lst->content);
		lst = lst->next;
	}
}

/*
void print_string_content(void *content)
{
    if (content)
        printf("Node content: %s\n", (char *)content);
    else
        printf("Node content: (null)\n");
}

int main(void)
{
    t_list *head = NULL;
    t_list *node1, *node2, *node3;

    char *str_data1 = "First Node";
    char *str_data2 = "Second Node";
    char *str_data3 = "Third Node";

    node1 = ft_lstnew(str_data1);
    node2 = ft_lstnew(str_data2);
    node3 = ft_lstnew(str_data3);

    // Build the list: head -> node1 -> node2 -> node3 -> NULL
    head = node1;
    node1->next = node2;
    node2->next = node3;

	printf("--- Testing ft_lstiter with a populated list ---\n");
	ft_lstiter(head, print_string_content);

    printf("\n--- Testing ft_lstiter with a NULL list ---\n");
    ft_lstiter(NULL, print_string_content);
    printf("(Expected no output from ft_lstiter for NULL list)\n");

    printf("\n--- Testing ft_lstiter with a NULL function ---\n");
    ft_lstiter(head, NULL);
    printf("(Expected no output or crash from ft_lstiter for NULL function)\n");
	printf("\nAll tests complete.\n");
    return (0);
}
*/