/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:24:57 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/09 19:40:53 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
flags[0] = has_quotes, flags[1] = has_wildcard

Delimiter is never expanded because:
It's a pattern to match against input lines
Must be literally preserved as the user typed it
Otherwise the heredoc can't find where the content ends

 */
int	handle_word(char *line, int *i, t_token **tokens,
			t_minishell *minishell)
{
	int		flags[2];
	int		disable_expand;
	char	*full_word;
	char	*segment;

	flags[0] = FALSE;
	flags[1] = FALSE;
	disable_expand = previous_token_is_heredoc(*tokens);
	full_word = expand_tilde_prefix(line, i, minishell, disable_expand);
	while (line[*i] && !is_separator(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			flags[0] = TRUE;
			segment = handle_quoted_string(line, i, minishell, disable_expand);
		}
		else if (!disable_expand && line[*i] == '$'
			&& (line[*i + 1] == '"' || line[*i + 1] == '\''))
		{
			(*i)++;
			continue ;
		}
		else
		{
			segment = get_unquoted_segment(line, i, minishell, disable_expand);
			if (!disable_expand && segment && (ft_strchr(segment, '*')
					|| ft_strchr(segment, '?')))
				flags[1] = TRUE;
		}
		full_word = strjoin_free(full_word, segment);
		if (!full_word)
			return (FALSE);
	}
	create_and_add_word_token(full_word, flags, tokens);
	free(full_word);
	(*i)--;
	return (TRUE);
}

int	previous_token_is_heredoc(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (FALSE);
	current = tokens;
	while (current->next)
		current = current->next;
	return (current->type == TOKEN_HEREDOC);
}

/*
Checking ~ condition :
~ | cat     # Followed by separator (space)
~/folder    # Followed by /
~           # Followed by nothing (\0)
 */
char	*expand_tilde_prefix(char *line, int *i, t_minishell *minishell,
	int disable_expand)
{
	char	*home;

	if (!disable_expand && line[*i] == '~' && (is_separator(line[*i + 1])
			|| line[*i + 1] == '/' || line[*i + 1] == '\0'))
	{
		home = get_var_value("HOME", minishell);
		(*i)++;
		if (!home || home[0] == '\0')
		{
			free(home);
			return (ft_strdup("~"));
		}
		return (home);
	}
	return (ft_strdup(""));
}

// Check if char is a separator
int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

char	*get_var_value(char *var_name, t_minishell *minishell)
{
	t_env	*current;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(minishell->last_exit_status));
	current = minishell->env_list;
	while (current)
	{
		if (ft_strncmp(current->key, var_name, ft_strlen(var_name) + 1) == 0)
		{
			if (current->value == NULL)
				return (ft_strdup(""));
			return (ft_strdup(current->value));
		}
		current = current->next;
	}
	return (ft_strdup(""));
}


