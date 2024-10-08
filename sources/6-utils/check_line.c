/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:42:03 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:18:35 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipe_control(char *line, int i);

int	check_line(void)
{
	t_minishell	*minishell;
	char		*line;
	int			quote;

	minishell = get_minishell();
	line = ft_strtrim(minishell->line, " ");
	if (!ft_strcmp(line, ""))
		return (gfree(line), FAILURE);
	quote = check_quote(line, ft_strlen(line));
	if (quote)
	{
		gfree(line);
		minishell->value.exit_code = 2;
		return (ft_dprintf(STD_ERROR, SYNTAX_ERR "%c\'\n", quote), FAILURE);
	}
	if (pipe_control(line, 0))
		return (FAILURE);
	return (SUCCESS);
}

int	check_quote(char *line, int value)
{
	char	quote;
	int		i;

	i = 0;
	quote = 0;
	while (i < value)
	{
		if (line[i] == S_QUOTE || line[i] == D_QUOTE)
		{
			if (quote == 0)
				quote = line[i];
			else if (quote == line[i])
				quote = 0;
		}
		i++;
	}
	return ((int)quote);
}

static int	check_pipe(char *line, int i);

static int	pipe_control(char *line, int i)
{
	t_minishell	*minishell;
	int			pipe;

	pipe = 0;
	minishell = get_minishell();
	while (line[i])
	{
		if (check_pipe(line, i))
			return (minishell->value.exit_code = 2, FAILURE);
		if (pipe == FALSE && line[i] == PIPE && !check_quote(line, i))
			pipe = TRUE;
		else if (line[i] != SPACE && ft_isprint(line[i]))
			pipe = FALSE;
		else if (line[i] == PIPE)
			break ;
		i++;
	}
	gfree(line);
	if (pipe)
	{
		minishell->value.exit_code = 2;
		return (ft_dprintf(STD_ERROR, ERR_TITLE SYNTAX_ERR " `|\'\n"), FAILURE);
	}
	return (SUCCESS);
}

static int	check_pipe(char *line, int i)
{
	if (!check_quote(line, i))
	{
		if (line[i] == PIPE && line[i + 1] == PIPE && line[i + 1])
		{
			ft_dprintf(STD_ERROR, ERR_TITLE SYNTAX_ERR " `||\'\n");
			return (FAILURE);
		}
		else if (line[0] == PIPE)
		{
			ft_dprintf(STD_ERROR, ERR_TITLE SYNTAX_ERR " `|\'\n");
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

t_list	*search_env(t_minishell *minishell, char *key)
{
	t_list	*cur_env;
	char	*env_key;

	if (key == NULL || ft_strlen(key) == 0)
		return (NULL);
	cur_env = minishell->env;
	while (cur_env)
	{
		env_key = ft_substr(cur_env->content, 0, get_key(cur_env->content));
		if (!ft_strcmp(env_key, key))
		{
			gfree(env_key);
			return (cur_env);
		}
		gfree(env_key);
		cur_env = cur_env->next;
	}
	return (NULL);
}
