/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:56:11 by ytop              #+#    #+#             */
/*   Updated: 2024/10/03 14:25:21 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

//aklımdasın

static void	remove_quotes_helper(char **tmp2, char **tmp, char **new_tmp)
{
	int	k;
	int	quote;

	k = -1;
	quote = 0;
	while ((*tmp2)[++k])
	{
		if (quote == 0 && ((*tmp2)[k] == '\'' || (*tmp2)[k] == '\"'))
			quote = (*tmp2)[k];
		else if (quote != 0 && quote == (*tmp2)[k])
			quote = 0;
		else
		{
			*new_tmp = strjoin_char(*tmp, (*tmp2)[k]);
			if (!(*new_tmp))
				return ;
			*tmp = *new_tmp;
		}
	}
}

void	remove_quotes(t_parser *parser)
{
	t_parser	*parser_tmp;
	char		*new_tmp;
	char		*tmp_one;
	char		*tmp_two;
	int			i;

	parser_tmp = parser;
	while (parser_tmp)
	{
		i = 0;
		while (parser_tmp->args[i])
		{
			tmp_one = ft_strdup("");
			if (!tmp_one)
				return ;
			tmp_two = parser_tmp->args[i];
			remove_quotes_helper(&tmp_two, &tmp_one, &new_tmp);
			parser_tmp->args[i] = tmp_one;
			i++;
		}
		parser_tmp = parser_tmp->next;
	}
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

void	ft_all_lower(char **str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			str[i][j] = ft_tolower(str[i][j]);
			j++;
		}
		i++;
	}
}

int	check_builtin(t_minishell *minishell, char **cmd, t_parser *parser)
{
	if (cmd[0] == NULL)
		return (FAILURE);
	if (!ft_strcmp(cmd[0], "env") && print_env())
		minishell->value.exit_code = 0;
	else if (!ft_strcmp(cmd[0], "export") && export(minishell, parser->args))
		minishell->value.exit_code = 0;
	else if (!ft_strcmp(cmd[0], "pwd") && get_pwd())
		minishell->value.exit_code = 0;
	else if (!ft_strcmp(cmd[0], "exit") && ft_exit(minishell, parser->args))
		minishell->value.exit_code = 0;
	else if (!ft_strcmp(cmd[0], "cd") && cd(minishell, parser->args[0]))
	{
		if (minishell->value.exit_code != 1)
			minishell->value.exit_code = 0;
	}
	else if (!ft_strcmp(cmd[0], "echo") && echo(parser->args))
		minishell->value.exit_code = 0;
	else if (!ft_strcmp(cmd[0], "unset") && unset(minishell, parser->args))
	{
		if (minishell->value.exit_code != 1)
			minishell->value.exit_code = 0;
	}
	else
		return (0);
	return (1);
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
	path_split = ft_split(path_list->content + 5, ':');
	path_cmd = search_path(path_split, temp);
	if (path_cmd)
		return (path_cmd);
	minishell->value.sign = 0;
	return (gfree(temp), cmd);
}

char	*handle_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] != S_QUOTE && str[i] != D_QUOTE)
		{
			result[j++] = str[i];
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	type_control(t_parser *parser, char **envs)
{
	if (!ft_strncmp(parser->args[0], "./", 2) || !ft_strncmp(parser->args[0], "/", 1))
	{
		if (execve(parser->args[0], parser->args, envs) == -1)
		{
			err_msg(NULL, parser->args[0], ": No such file or directory");
			exit(127);
		}
	}
	return (FAILURE);
}

char	**env();

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
			err_msg(NULL, minishell->path, ": command not found");
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
