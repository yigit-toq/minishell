/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 00:37:09 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 00:37:09 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	check_pid(t_parser *parser, pid_t *pid)
{
	t_minishell	*minishell;
	char		**envs;

	minishell = get_minishell();
	envs = env();
	if (!(*pid))
	{
		if (execve(minishell->path, parser->args, envs) == -1)
		{
			if (!type_control(parser, envs))
				exit(EXIT_FAILURE);
			err_msg(NULL, minishell->path, "command not found");
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
	t_list		*env_list;
	char		**envs;
	int			index;

	env_list = get_minishell()->env;
	envs = ft_calloc(ft_lstsize(env_list) + 1, sizeof(char *));
	if (!envs)
		return (perror("ft_calloc"), NULL);
	index = 0;
	while (env_list)
	{
		envs[index++] = env_list->content;
		env_list = env_list->next;
	}
	return (envs);
}

char	*search_path(char **path_split, char *temp)
{
	t_minishell	*minishell;
	char		*path_cmd;
	int			index;

	index = 0;
	minishell = get_minishell();
	while (path_split[index])
	{
		path_cmd = ft_strjoin(path_split[index], temp);
		if (access(path_cmd, F_OK) == 0)
			return (minishell->value.sign = 1, path_cmd);
		gfree(path_cmd);
		index++;
	}
	return (NULL);
}

char	*find_path(char *cmd)
{
	t_minishell	*minishell;
	t_list		*path_list;
	char		*temp_cmd;
	char		*path_cmd;
	char		**path_split;

	temp_cmd = ft_strjoin("/", cmd);
	if (!temp_cmd)
		return (NULL);
	minishell = get_minishell();
	path_list = search_env(minishell, "PATH");
	if (!path_list)
		return (minishell->value.sign = 0, gfree(temp_cmd), cmd);
	path_split = ft_split(path_list->content + 5, ':');
	path_cmd = search_path(path_split, temp_cmd);
	if (path_cmd)
		return (path_cmd);
	minishell->value.sign = 0;
	return (gfree(temp_cmd), cmd);
}

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
