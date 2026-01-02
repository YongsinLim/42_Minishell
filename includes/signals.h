/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 22:07:49 by jenlee            #+#    #+#             */
/*   Updated: 2026/01/02 18:56:41 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"

/* ---- Signal Initializers ---- */
void	init_signals_prompt(void);
void	init_signals_child(void);
void	init_signals_heredoc(void);

/* ---- Signal Handlers ---- */
void	sigint_handler_prompt(int sig);
void	sigint_handler_heredoc(int sig);

#endif