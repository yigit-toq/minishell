/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:38:29 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 17:44:36 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/dir.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

static int	check_builtin2(t_minishell *minishell, char **cmd,
			t_parser *parser, int *i)
{
	if (ft_strcmp(cmd[*i], "cd") == 0)
	{
		cd(minishell, parser->args[1]);
		if (minishell->value.exit_code != 1)
			minishell->value.exit_code = 0;
	}
	else if (ft_strcmp(cmd[*i], "echo") == 0)
	{
		echo(parser->args);
		minishell->value.exit_code = 0;
	}
	else if (ft_strcmp(cmd[*i], "unset") == 0)
	{
		unset(minishell, parser->args);
		if (minishell->value.exit_code != 1)
			minishell->value.exit_code = 0;
	}
	else
		return (0);
	reset_fd();
	return (1);
}

int	check_builtin(char **cmd, t_parser *parser, int *i)
{
	t_minishell	*shell;

	shell = get_minishell();
	if (cmd[*i] == NULL)
		return (FAILURE);
	if (ft_strcmp(cmd[*i], "env") == 0)
	{
		print_env();
		shell->value.exit_code = 0;
	}
	else if (ft_strcmp(cmd[*i], "export") == 0)
	{
		shell->value.exit_code = 0;
		export(shell, parser->args);
	}
	else if (ft_strcmp(cmd[*i], "pwd") == 0 && get_pwd())
		shell->value.exit_code = 0;
	else if (ft_strcmp(cmd[*i], "exit") == 0)
	{
		ft_exit(shell, parser->args);
		shell->value.exit_code = 1;
	}
	else
		return (check_builtin2(shell, cmd, parser, i));
	return (reset_fd(), 1);
}

static void	arg_type(char *arg)
{
	t_minishell	*minishell;
	DIR			*dir;

	minishell = get_minishell();
	dir = opendir(arg);
	if ((dir) != NULL)
	{
		closedir(dir);
		ft_putstr_fd(arg, STD_ERROR);
		ft_putstr_fd(": is a directory\n", STD_ERROR);
		minishell->value.exit_code = 126;
	}
	else if (access(arg, F_OK) == -1)
	{
		ft_putstr_fd(arg, STD_ERROR);
		ft_putstr_fd(": No such file or directory\n", STD_ERROR);
		minishell->value.exit_code = 127;
	}
	else if (access(arg, X_OK) == -1)
	{
		ft_putstr_fd(arg, STD_ERROR);
		ft_putstr_fd(": Permission denied\n", STD_ERROR);
		minishell->value.exit_code = 126;
	}
}

int	type_control(t_parser *parser, char **envs)
{
	t_minishell	*minishell;
	int			i;

	minishell = get_minishell();
	if (!ft_strncmp(parser->args[0], "./", 2)
		|| !ft_strncmp(parser->args[0], "/", 1))
	{
		if (execve(parser->args[0], parser->args, envs) == -1)
		{
			arg_type(parser->args[0]);
			if (minishell->value.sign)
				gfree(minishell->path);
			i = minishell->value.exit_code;
			return (clear_garbage(), rl_clear_history(), exit(i), 0);
		}
	}
	return (FAILURE);
}
