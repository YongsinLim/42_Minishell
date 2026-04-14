/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_sets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 22:09:35 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 17:38:42 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"

volatile sig_atomic_t g_signal = 0;

void	sigint_handler_prompt(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_handler_heredoc(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

void	sigquit_handler_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(3);
}
