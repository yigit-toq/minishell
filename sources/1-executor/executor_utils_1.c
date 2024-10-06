/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:56:11 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 17:33:45 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <dirent.h>

static int	check_builtin_utils(char **cmd, t_parser *parser, int *i)
{
	t_minishell	*minishell;

	minishell = get_minishell();
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
		return (SUCCESS);
	reset_fd();
	return (FAILURE);
}

int	check_builtin(char **cmd, t_parser *parser, int *i)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	if (cmd[*i] == NULL)
		return (FAILURE);
	if (ft_strcmp(cmd[*i], "env") == 0)
	{
		print_env();
		minishell->value.exit_code = 0;
	}
	else if (ft_strcmp(cmd[*i], "exit") == 0)
	{
		ft_exit(minishell, parser->args);
		minishell->value.exit_code = 0;
	}
	else if (ft_strcmp(cmd[*i], "export") == 0)
	{
		minishell->value.exit_code = 0;
		export(minishell, parser->args);
	}
	else if (ft_strcmp(cmd[*i], "pwd") == 0 && get_pwd())
		minishell->value.exit_code = 0;
	else
		return (check_builtin_utils(cmd, parser, i));
	return (reset_fd(), FAILURE);
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
		ft_dprintf(STD_ERROR, "%s: is a directory\n", arg);
		minishell->value.exit_code = 126;
	}
	else if (access(arg, X_OK) == -1)
	{
		ft_dprintf(STD_ERROR, "%s: Permission denied\n", arg);
		minishell->value.exit_code = 126;
	}
	else if (access(arg, F_OK) == -1)
	{
		ft_dprintf(STD_ERROR, "%s: No such file or directory\n", arg);
		minishell->value.exit_code = 127;
	}
}

int	type_control(t_parser *parser, char **envs)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	if (!ft_strncmp(parser->args[0], "./", 2)
		|| !ft_strncmp(parser->args[0], "/", 1))
	{
		if (execve(parser->args[0], parser->args, envs) == -1)
		{
			arg_type(parser->args[0]);
			if (minishell->value.sign)
				gfree(minishell->path);
			return (exit(minishell->value.exit_code), 0);
		}
	}
	return (FAILURE);
}
