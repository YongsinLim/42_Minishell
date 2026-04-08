/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_sets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 22:09:35 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/08 14:15:35 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/signals.h"

volatile sig_atomic_t g_heredoc_interrupted = 0;

void	sigint_handler_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	g_heredoc_interrupted = 1;
}

void	sigquit_handler_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(3);
}
