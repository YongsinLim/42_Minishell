/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 14:38:17 by yolim             #+#    #+#             */
/*   Updated: 2026/04/16 14:41:25 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
rl_clear_history() is used to completely clear the command history list and
free the memory associated with its entries
*/
void	cleanup_and_exit(t_minishell *minishell, int exit_status)
{
	free_env_list(minishell->env_list);
	free(minishell->input);
	free_history(&minishell->history_list);
	free_tokens(&minishell->tokens);
	free_ast(&minishell->ast);
	rl_clear_history();
	exit(exit_status);
}
