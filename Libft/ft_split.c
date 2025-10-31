/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:30:09 by yolim             #+#    #+#             */
/*   Updated: 2025/06/01 13:22:35 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s != '\0')
	{
		if ((*s != c) && (in_word == 0))
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static void	free_split_array(char **arr, size_t count)
{
	size_t	k;

	k = 0;
	while (k < count)
	{
		free(arr[k]);
		k++;
	}
	free(arr);
}

static int	populate_split_array(char **result_array, char const *s, char c,
				size_t num_words_actual)
{
	size_t	i;
	size_t	j;
	size_t	word_start;

	i = 0;
	j = 0;
	while (j < num_words_actual && s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			word_start = i;
			while (s[i] && s[i] != c)
				i++;
			result_array[j] = ft_substr(s, word_start, i - word_start);
			if (result_array[j] == NULL)
				free_split_array(result_array, j);
			j++;
		}
	}
	result_array[j] = NULL;
	return (0);
}

char	**ft_split(char const *s, char c)
{
	size_t	num_words_actual;
	char	**result_array;

	if (s == NULL)
		return (NULL);
	num_words_actual = count_words(s, c);
	result_array = (char **) malloc((num_words_actual + 1) * sizeof(char *));
	if (result_array == NULL)
		return (NULL);
	populate_split_array(result_array, s, c, num_words_actual);
	return (result_array);
}

/*
int	main(void)
{
	char **result_array;
	const char *s = "123,4567, ,891011,,121311415";
	char delimiter = ',';
	int i;

	result_array = ft_split(s, delimiter);

	if (result_array == NULL)
	{
		printf("ft_split returned NULL.");
		printf("Could be due to NULL input or memory allocation failure.\n");
		return (1);
	}

	printf("Splitting string \"%s\" with delimiter '%c':\n", s, delimiter);
	i = 0;
	while (result_array[i] != NULL)
	{
		printf("Index : %d\n", i);
		printf("Result : %s\n", result_array[i]);
		free(result_array[i]);
		result_array[i] = NULL;
		i++;
	}
	free(result_array);
	result_array = NULL;

	return (0);
}
*/