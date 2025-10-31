/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:55:04 by yolim             #+#    #+#             */
/*   Updated: 2025/09/14 14:02:42 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
#include <stdio.h> // Required for printf
#include <limits.h> // Required for INT_MAX

	"..." (accept any nbr of args)
	va_list - hold info needed to retrieve additional args. think like a
	pointer-like object that keep track the position in argument list.

	va_start() is to initialize va_list object point to 1st variable arguments
	inside the "...".

	call va_args() is to retrieve the next argument from the list & move
	va_list to next args. must specify the type of args that retrieving.
	get value - va_arg(args, <variable type>)

	va_copy() is to creates a copy of a va_list.
	Usage: when you need to traverse the argument list multiple times or
	pass it to another function.

	va_end() is to clean up the va_list when done. Must call this after every
	call of va_start & va_copy.

	e.g. ft_printf(const char *str, ...)
	... can accept 0 to n args, ft_printf("%s abc %s", "aaa", " ")
	call va_start() will point to "aaa"
	call va_args to get "aaa" value, and call again will get " ".

	va_args(int type): default argument promotion rules,
	purpose: no need to handle many different smaller, non-std sizes
	
	why pointer used in va_list *args in printarg_getcount():
	In AMD, without pointer, still work as expected, va_list can still track
	the correct position after the va_args() call in different times of loop

	In ARM, without pointer, va_list when pass to another function (not within
	same function (scope) with va_start),it always point to the 1st args.
	if in another function call va_args by pass va_list to the function will
	get the same result of args (which is the 1st args).
	in order 
*/
static int	printarg_getcount(const char *str, va_list *args)
{
	int	count;

	count = 0;
	if (*str == 'c')
		count = ft_putchar_getcount((char) va_arg(*args, int));
	else if (*str == '%')
		count = ft_putchar_getcount('%');
	else if (*str == 's')
		count = ft_putstr_getcount(va_arg(*args, char *));
	else if ((*str == 'd') || (*str == 'i'))
		count = ft_putint_getcount(va_arg(*args, int));
	else if ((*str == 'u') || (*str == 'x') || (*str == 'X'))
		count = ft_put_unbr_getcount(va_arg(*args, unsigned int), *str);
	else if (*str == 'p')
		count = ft_putptr_getcount((uintptr_t) va_arg(*args, void *));
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		total_printed_chars;
	int		count;

	total_printed_chars = 0;
	va_start(args, str);
	while (*str != '\0')
	{
		while ((*str != '%') && (*str != '\0'))
			total_printed_chars += ft_putchar_getcount(*str++);
		if (*str == '\0')
			break ;
		else
			count = printarg_getcount(++str, &args);
		if (count == -1)
		{
			va_end(args);
			return (-1);
		}
		total_printed_chars = total_printed_chars + count;
		str++;
	}
	va_end(args);
	return (total_printed_chars);
}

/*
int	main()
{
	int ft_ret;
	int std_ret;

	printf("Starting ft_printf tests...\n");
	printf("========================================\n");

	// Test 1: Simple string without specifiers
	printf("\n--- Test 1: Simple String ---\n");
	ft_ret = ft_printf("Hello, World!\n");
	std_ret = printf("Hello, World!\n");
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n", std_ret);

	// Test 2: Character %c
	printf("\n--- Test 2: %%c ---\n");
	char c_test = 'A';
	ft_ret = ft_printf("ft_printf char: %c\n", c_test);
	std_ret = printf("   printf char: %c\n", c_test);
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n", std_ret);

	ft_ret = ft_printf("ft_printf char (0): %c!\n", '\0');
	// Note: printing \0 behavior
	std_ret = printf("   printf char (0): %c!\n", '\0');
	printf("ft_printf returned (char 0): %d\n", ft_ret);
	printf("   printf returned (char 0): %d\n", std_ret);

	// Test 3: String %s
	printf("\n--- Test 3: %%s ---\n");
	char *s_test = "This is a test string.";
	char *s_null = NULL;
	ft_ret = ft_printf("ft_printf string: %s\n", s_test);
	std_ret = printf("   printf string: %s\n", s_test);
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n\n", std_ret);

	ft_ret = ft_printf("ft_printf NULL string: %s\n", s_null);
	std_ret = printf("   printf NULL string: %s\n", s_null);
	printf("ft_printf returned (NULL): %d\n", ft_ret);
	printf("   printf returned (NULL): %d\n\n", std_ret);

	ft_ret = ft_printf("ft_printf empty string: %s\n", "");
	std_ret = printf("   printf empty string: %s\n", "");
	printf("ft_printf returned (Empty): %d\n", ft_ret);
	printf("   printf returned (Empty): %d\n", std_ret);

	// Test 4: Pointer %p
	printf("\n--- Test 4: %%p ---\n");
	int num_ptr = 42;
	void *p_test = &num_ptr;
	void *p_null = NULL;
	ft_ret = ft_printf("ft_printf pointer: %p\n", p_test);
	std_ret = printf("   printf pointer: %p\n", p_test);
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n\n", std_ret);

	ft_ret = ft_printf("ft_printf NULL pointer: %p\n", p_null);
	std_ret = printf("   printf NULL pointer: %p\n", p_null);
	printf("ft_printf returned (NULL ptr): %d\n", ft_ret);
	printf("   printf returned (NULL ptr): %d\n", std_ret);

	// Test 5: Decimal %d and Integer %i
	printf("\n--- Test 5: %%d and %%i ---\n");
	ft_ret = ft_printf("ft_printf Decimals: %d, %i, %d\n", 12345, -67890, 0);
	std_ret = printf("   printf Decimals: %d, %i, %d\n", 12345, -67890, 0);
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n\n", std_ret);

	ft_ret = ft_printf("ft_printf INT_MAX: %d\n", INT_MAX);
	std_ret = printf("   printf INT_MAX: %d\n", INT_MAX);
	printf("ft_printf returned (INT_MAX): %d\n", ft_ret);
	printf("   printf returned (INT_MAX): %d\n\n", std_ret);

	ft_ret = ft_printf("ft_printf INT_MIN: %d\n", INT_MIN);
	std_ret = printf("   printf INT_MIN: %d\n", INT_MIN);
	printf("ft_printf returned (INT_MIN): %d\n", ft_ret);
	printf("   printf returned (INT_MIN): %d\n", std_ret);

	// Test 6: Unsigned Decimal %u
	printf("\n--- Test 6: %%u ---\n");
	ft_ret = ft_printf("ft_printf Unsigned: %u, %u, %u\n", 12345u, 0u, UINT_MAX);
	std_ret = printf("   printf Unsigned: %u, %u, %u\n", 12345u, 0u, UINT_MAX);
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n", std_ret);

	// Test 7: Percent Sign %%
	printf("\n--- Test 9: %%%% ---\n");
	ft_ret = ft_printf("ft_printf Percent: %%\n");
	std_ret = printf("   printf Percent: %%\n");
	printf("ft_printf returned: %d\n", ft_ret);
	printf("   printf returned: %d\n", std_ret);

	printf("\n========================================\n");
	printf("Finished ft_printf tests.\n");

	return (0);
}
*/