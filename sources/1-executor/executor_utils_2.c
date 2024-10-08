/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:56:11 by ytop              #+#    #+#             */
/*   Updated: 2024/10/08 16:43:43 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>

void	init_cmd(t_parser *parser, char **cmd)
{
	t_parser	*parser_tmp;
	int			i;

	i = 0;
	parser_tmp = parser;
	while (parser_tmp)
	{
		if (parser_tmp->args[0])
		{
			cmd[i] = parser_tmp->args[0];
			i++;
		}
		parser_tmp = parser_tmp->next;
	}
}

char	*search_path(char **path_split, char *temp)
{
	t_minishell	*minishell;
	char		*path_cmd;
	int			i;

	i = 0;
	minishell = get_minishell();
	while (path_split[i])
	{
		path_cmd = ft_strjoin(path_split[i], temp);
		if (access(path_cmd, F_OK) == 0)
			return (minishell->value.sign = 1, path_cmd);
		gfree(path_cmd);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd)
{
	t_minishell	*minishell;
	t_list		*path_list;
	char		**path_split;
	char		*path_cmd;
	char		*temp;

	temp = ft_strjoin("/", cmd);
	if (!temp)
		return (NULL);
	minishell = get_minishell();
	path_list = search_env(minishell, "PATH");
	if (!path_list)
	{
		minishell->value.sign = 0;
		return (gfree(temp), cmd);
	}
	path_split = ft_split(path_list->content + 5, ':');
	path_cmd = search_path(path_split, temp);
	if (path_cmd)
		return (path_cmd);
	minishell->value.sign = 0;
	return (gfree(temp), cmd);
}

void	check_pid(t_minishell *minishell, t_parser *parser, pid_t *pid)
{
	char		**envs;

	envs = env();
	if (!(*pid))
	{
		if (execve(minishell->path, parser->args, envs) == -1)
		{
			if (!type_control(parser, envs))
				exit(EXIT_FAILURE);
			err_msg(NULL, minishell->path, "command not found");
			clear_garbage();
			rl_clear_history();
			exit(127);
		}
	}
	else
	{
		waitpid(*pid, &minishell->value.exit_code, 0);
		minishell->value.exit_code = WEXITSTATUS(minishell->value.exit_code);
		if (g_signal == 0)
			minishell->value.exit_code = 130;
		else if (g_signal == 4)
			minishell->value.exit_code = 131;
		reset_fd();
		gfree(envs);
	}
}

char	**env(void)
{
	t_minishell	*minishell;
	t_list		*env_list;
	char		**envs;
	int			i;

	minishell = get_minishell();
	env_list = minishell->env;
	i = 0;
	envs = ft_calloc(ft_lstsize(env_list) + 1, sizeof(char *));
	if (!envs)
		return (perror("ft_calloc"), NULL);
	while (env_list)
	{
		envs[i++] = env_list->content;
		env_list = env_list->next;
	}
	return (envs);
}
