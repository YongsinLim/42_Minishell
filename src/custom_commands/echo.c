/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:40:55 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/11 17:40:55 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*Check Newlne*/
int should_print_newline(char **argv)
{
    int i;
    int j;
    int e_newline;

	i = 1;
	e_newline = 1;
    while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
    {
        j = 1;
        while (argv[i][j] == 'n')
            j++;
        if (argv[i][j] == '\0')
        {
            e_newline = 0;
            i++;
        }
        else
            break;
    }
    return (e_newline);
}

/* Function to print the arguments */
void print_arguments(char **argv, int e_newline)
{
    int i;
	
	i = 1;
    while (argv[i])
    {
        ft_putstr_fd(argv[i], 1);
        if (argv[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (e_newline)
        ft_putstr_fd("\n", 1);
}

/* Main echo function */
int ft_echo(char **argv)
{
    int e_newline;

    e_newline = should_print_newline(argv);
    print_arguments(argv, e_newline);
    return (0);
}
