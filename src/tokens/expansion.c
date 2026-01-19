/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 10:14:40 by yolim             #+#    #+#             */
/*   Updated: 2026/01/19 16:26:48 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variable(char *str, char **envp)
{
	char	*final_str;
	int		i;

	final_str = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			final_str = handle_dollar_sign(str, &i, final_str, envp);
		else
		{
			final_str = append_char(final_str, str[i]);
			i++;
		}
	}
	return (final_str);
}

char	*handle_dollar_sign(char *str, int *i_ptr, char *final_str, char **envp)
{
	char	*var_name;
	char	*var_value;
	char	*old_final_str;
	int		var_name_len;

	var_name = NULL;
	var_name_len = get_var_name_len(&str[*i_ptr + 1], &var_name);
	if (var_name_len > 0)
	{
		var_value = get_var_value(var_name, envp);
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

char	*get_var_value(char *var_name, char **envp)
{
	char	*var_value;

	(void)envp;
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_strdup("0"));
	var_value = getenv(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}
//ft_strdup("0")-temporary placeholder, will return real exit status once ready.

/*
if dont add envp as parameter:
   * getenv() reads from the environment that your minishell process started
   		with. It cannot see changes made "internally" by a command like export.
   * When your export command runs, it will modify an internal list of
   		variables, but it cannot modify the `getenv` database.
   * So, when echo $MY_VAR is processed, your expand_variable function will call
     getenv("MY_VAR"), which will return NULL (not found), because the original
	 environment doesn't have MY_VAR in it. Your shell will incorrectly print an
	 empty line.
*/