/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:26:04 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/12 18:59:41 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	"." = current directory

	Bash Behaviour :
	If User types: echo *.xyz (No .xyz files exist)
	Result: prints "*.xyz" literally

	Open: Use opendir() to get a directory stream.
	Read: Use readdir() in a loop to retrieve each entry (a struct dirent *)
			until it returns NULL.
			readdir(dir) returns one directory entry at a time and advances
			an internal cursor.
	Close: Use closedir() to close the stream and release the underlying file
			descriptor.

	if (entry->d_name[0] != '.' || pattern[0] == '.') do:
	* include normal files always (entry->d_name[0] != '.'),
	* include hidden files only if user key in pattern starts with . (eg .env*).
*/
void	expand_wildcard(char *pattern, t_list **argv_list)
{
	DIR				*dir;
	int				match_count;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return ;
	match_count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if ((entry->d_name[0] != '.' || pattern[0] == '.')
			&& match_pattern(pattern, entry->d_name))
		{
			if (add_arg_node(argv_list, entry->d_name) == FALSE)
				return ((void)closedir(dir));
			match_count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (match_count > 0)
		sort_argv_list(argv_list);
	else
		add_arg_node(argv_list, pattern);
}

/*
'*' can match "zero or more characters"
'?' can match exactly one character
normal characters = must match exactly

1st: check BOTH reached the end (Perfect full match)
2nd: check if pattern reached end but filename hasn't
3rd: check if filename reached end but pattern hasn't
	(Remaining pattern must be all * to then only match with empty tail
	(e.g. pattern is "abc*" can match filename is "abc").)
4th: check when current *pattern is *
	 1. match_pattern(pattern + 1, filename) → Skip '*' (match 0 characters)
	 2. match_pattern(pattern, filename + 1) → Keep '*' (match 1+ characters)
5th: check when current *pattern is ?
	 (1 char in pattern must match 1 char in filename)
6th: check regular character must match exactly

e.g.
match_pattern("*.c", "main.c") → 1
match_pattern("a?c", "abc") → 1
match_pattern("a?c", "ac") → 0 (? needs one char)
match_pattern("ab*", "ab") → 1
match_pattern("ab*", "abxyz") → 1
match_pattern("*.xyz", "file.txt") → 0
*/
int	match_pattern(char *pattern, char *filename)
{
	if (*pattern == '\0' && *filename == '\0')
		return (TRUE);
	if (*pattern == '\0' && *filename != '\0')
		return (FALSE);
	if (*filename == '\0')
	{
		while (*pattern != '\0')
		{
			if (*pattern != '*')
				return (FALSE);
			pattern++;
		}
		return (TRUE);
	}
	if (*pattern == '*')
	{
		if (match_pattern(pattern + 1, filename))
			return (TRUE);
		if (match_pattern(pattern, filename + 1))
			return (TRUE);
		return (FALSE);
	}
	if (*pattern == '?' || *pattern == *filename)
		return (match_pattern(pattern + 1, filename + 1));
	return (FALSE);
}

int	add_arg_node(t_list **argv_list, char *value)
{
	char	*filename_copy;
	t_list	*new_node;

	filename_copy = ft_strdup(value);
	if (!filename_copy)
		return (FALSE);
	new_node = ft_lstnew(filename_copy);
	if (!new_node)
	{
		free(filename_copy);
		return (FALSE);
	}
	ft_lstadd_back(argv_list, new_node);
	return (TRUE);
}

void	sort_argv_list(t_list **argv_list)
{
	int		swapped;
	t_list	*current;
	t_list	*next;
	char	*temp;

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
			if (compare_filename(current->content, next->content) > 0)
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
