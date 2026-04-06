/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:26:04 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/06 12:46:35 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// '*' can match "zero or more characters"
// '?' can match exactly one character
int	match_pattern(char *pattern, char *filename)
{
	// Check if BOTH reached the end
	if (*pattern == '\0' && *filename == '\0')
		return (1);
	// Check if pattern reached end but filename hasn't
	if (*pattern == '\0' && *filename != '\0')
		return (0); // Pattern too short to match
	// Check if filename reached end but pattern hasn't
	if (*filename == '\0')
	{
		// Only match if remaining pattern is all '*'
		while (*pattern != '\0')
		{
			if (*pattern != '*')
				return (0);
			pattern++;
		}
		return (1);
	}
	// RECURSIVE CASE 1: Pattern has '*'
	if (*pattern == '*')
	{
		// Option 1: Skip '*' (match 0 characters)
		if (match_pattern(pattern + 1, filename))
			return (1);
		// Option 2: Keep '*' (match 1+ characters)
		if (match_pattern(pattern, filename + 1))
			return (1);
		return (0);
	}
	// RECURSIVE CASE 2: Pattern has '?'
	// '?' matches exactly one character (any character)
	if (*pattern == '?')
		return (match_pattern(pattern + 1, filename + 1));
	// RECURSIVE CASE 3: Regular character must match exactly
	if (*pattern == *filename)
		return (match_pattern(pattern + 1, filename + 1));
	return (0);
}

int	compare_filename(char *a, char *b)
{
	while (*a && *b)
	{
		if (*a != *b)
			return (*a - *b);
		a++;
		b++;
	}
	if (*a || *b)
		return (*a - *b);
	return (0);
}

void	sort_argv_list(t_list **argv_list)
{
	t_list	*current;
	t_list	*next;
	char	*temp;
	int		swapped;

	if (!argv_list || !*argv_list)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = *argv_list;
		while (current && current->next)
		{
			next = current->next;
			if (compare_filename((char *)current->content,
					(char *)next->content) > 0)
			{
				temp = (char *)current->content;
				current->content = next->content;
				next->content = temp;
				swapped = 1;
			}
			current = current->next;
		}
	}
}

/*
	"." = current directory

	User types: echo *.xyz
	No .xyz files exist
	Result: prints "*.xyz" literally
*/
void	expand_wildcard(char *pattern, t_list **argv_list)
{
	DIR				*dir;
	struct dirent	*entry;
	int				match_count;
	char			*filename_copy;

	dir = opendir(".");
	if (!dir)
		return ;
	match_count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' || pattern[0] == '.')
		{
			if (match_pattern(pattern, entry->d_name))
			{
				filename_copy = ft_strdup(entry->d_name);
				if (!filename_copy)
				{
					closedir(dir);
					return ;
				}
				match_count++;
				ft_lstadd_back(argv_list, ft_lstnew(filename_copy));
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (match_count > 0)
		sort_argv_list(argv_list);
	if (match_count == 0)
	{
		filename_copy = ft_strdup(pattern);
		if (!filename_copy)
			return ;
		ft_lstadd_back(argv_list, ft_lstnew(filename_copy));
	}
}
