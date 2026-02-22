/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:41:28 by jenlee            #+#    #+#             */
/*   Updated: 2026/02/11 17:41:28 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Check if command is a builtin
int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strncmp(cmd, "echo", 5) == 0)
        return (1);
    if (ft_strncmp(cmd, "cd", 3) == 0)
        return (1);
    if (ft_strncmp(cmd, "pwd", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "export", 7) == 0)
        return (1);
    if (ft_strncmp(cmd, "unset", 6) == 0)
        return (1);
    if (ft_strncmp(cmd, "env", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "exit", 5) == 0)
        return (1);
    return (0);
}

int exec_builtin(char **argv, t_env **env_head)
{
    if (ft_strncmp(argv[0], "echo", 5) == 0)
        return (ft_echo(argv));
    if (ft_strncmp(argv[0], "pwd", 4) == 0)
        return (ft_pwd());
    if (ft_strncmp(argv[0], "env", 4) == 0)
        return (ft_env(*env_head));
    if (ft_strncmp(argv[0], "cd", 3) == 0)
        return (ft_cd(argv, env_head));
    if (ft_strncmp(argv[0], "export", 7) == 0)
        return (ft_export(argv, env_head));
    if (ft_strncmp(argv[0], "unset", 6) == 0)
        return (ft_unset(argv, env_head));
    if (ft_strncmp(argv[0], "exit", 5) == 0)
        return (0); // Placeholder for ft_exit(argv)

    return (1);
}