	/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
	/*   expansion.c                                        :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2026/01/12 10:14:40 by yolim             #+#    #+#             */
	/*   Updated: 2026/02/11 18:20:04 by jenlee           ###   ########.fr       */
	/*                                                                            */
	/* ************************************************************************** */

	#include "../../includes/minishell.h"

	char	*expand_variable(char *str, t_env *env)
	{
		char	*final_str;
		int		i;
		int		in_sq;
		int		in_dq;

		final_str = ft_strdup("");
		i = 0;
		in_sq = 0;
		in_dq = 0;
		while (str[i])
		{
			// 1. Track Quotes
			if (str[i] == '\'' && !in_dq)
				in_sq = !in_sq;
			else if (str[i] == '\"' && !in_sq)
				in_dq = !in_dq;
			if (str[i] == '$' && !in_sq)
				final_str = handle_dollar_sign(str, &i, final_str, env);
			else
			{
				final_str = append_char(final_str, str[i]);
				i++;
			}
		}
		return (final_str);
	}

	char	*handle_dollar_sign(char *str, int *i_ptr, char *final_str, t_env *env)
	{
		char	*var_name;
		char	*var_value;
		char	*old_final_str;
		int		var_name_len;

		var_name = NULL;
		var_name_len = get_var_name_len(&str[*i_ptr + 1], &var_name);
		if (var_name_len > 0) // Valid Variable is Found
		{
			var_value = get_var_value(var_name, env);
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

	char	*get_var_value(char *var_name, t_env *env) // <--- Takes t_env* now
	{
		char	*value;

		if (!var_name)
			return (ft_strdup(""));
		// Handle Exit Status
		if (ft_strncmp(var_name, "?", 2) == 0)
			return (ft_itoa(g_exit_status));
		value = get_env_val(env, var_name);
		if (value)
			return (ft_strdup(value));
		return (ft_strdup(""));
	}
