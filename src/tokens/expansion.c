/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 10:14:40 by yolim             #+#    #+#             */
/*   Updated: 2026/02/26 17:42:06 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variable(char *str, t_minishell *minishell)
{
	char	*final_str;
	int		i;

	final_str = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			final_str = handle_dollar_sign(str, &i, final_str, minishell);
		else
		{
			final_str = append_char(final_str, str[i]);
			i++;
		}
	}
	return (final_str);
}

char	*handle_dollar_sign(char *str, int *i_ptr, char *final_str,
	t_minishell *minishell)
{
	char	*var_name;
	char	*var_value;
	char	*old_final_str;
	int		var_name_len;

	var_name = NULL;
	var_name_len = get_var_name_len(&str[*i_ptr + 1], &var_name);
	if (var_name_len > 0)
	{
		var_value = get_var_value(var_name, minishell);
		old_final_str = final_str;
		final_str = ft_strjoin(final_str, var_value);
		free(old_final_str);
		free(var_value);
		free(var_name);
		*i_ptr += var_name_len + 1;
	}
	else
	{
		final_str = append_char(final_str, str[*i_ptr]);
		(*i_ptr)++;
	}
	return (final_str);
}

char	*append_char(char *s1, char c)
{
	char	*new_str;
	char	array[2];

	array[0] = c;
	array[1] = '\0';
	new_str = ft_strjoin(s1, array);
	free(s1);
	return (new_str);
}

int	get_var_name_len(char *str, char **var_name_ptr)
{
	int	i;

	if (str[0] == '?')
	{
		*var_name_ptr = ft_strdup("?");
		return (1);
	}
	if (!ft_isalpha(str[0]) && str[0] != '_')
	{
		*var_name_ptr = NULL;
		return (0);
	}
	i = 1;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	*var_name_ptr = ft_substr(str, 0, i);
	return (i);
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
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}
