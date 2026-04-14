/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:41:28 by jenlee            #+#    #+#             */
/*   Updated: 2026/04/14 17:56:54 by yolim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd || !cmd[0])
		return (FALSE);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (TRUE);
	if (ft_strncmp(cmd, "history", 8) == 0)
		return (TRUE);
	return (FALSE);
}

int	handle_builtin_execution(t_ast_node *ast, t_minishell *minishell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (redirect_input(ast->command) != SHELL_SUCCESS
		|| redirect_output(ast->command) != SHELL_SUCCESS)
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (SHELL_FAILURE);
	}
	status = execute_builtin(ast->command->argv, minishell);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

int	execute_builtin(char **argv, t_minishell *minishell)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (ft_echo(argv));
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (ft_cd(argv, minishell));
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (ft_export(argv, minishell));




	if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (ft_unset(argv, minishell));
	if (ft_strncmp(argv[0], "env", 4) == 0)
		return (ft_env(argv, minishell));
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (ft_exit(argv, minishell));
	if (ft_strncmp(argv[0], "history", 8) == 0)
		return (ft_history(argv, minishell->history_list));
	return (SHELL_FAILURE);
}
