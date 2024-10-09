/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:27:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:25:16 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	empty_dollar(char *line);

static void	replace_dollar(char *str, char **result, int *i, char *q);

static void	dollar_ext(char *arg, int i, char *q)
{
	if (!q[0] && arg[i] == S_QUOTE)
		q[0] = arg[i];
	else if (!q[1] && arg[i] == D_QUOTE)
		q[1] = arg[i];
	else if (q[0] && q[0] == arg[i])
		q[0] = 0;
	else if (q[1] && q[1] == arg[i])
		q[1] = 0;
}

char	*dollar(int i, int j, char *arg)
{
	char	*result;
	char	q[2];

	if (empty_dollar(arg))
		return (arg);
	ft_bzero(q, 2);
	result = ft_strdup("");
	while (arg[i])
	{
		dollar_ext(arg, i, q);
		if ((q[0] || (!q[0] && arg[i] == S_QUOTE)) && !j)
		{
			if (q[1])
			{
				j = 1;
				continue ;
			}
			result = strjoin_char(result, arg[i++]);
		}
		else
			replace_dollar(arg, &result, &i, q);
		j = 0;
	}
	return (result);
}

static int	empty_dollar(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == DOLLAR)
		{
			if (!ft_isalnum(line[i + 1])
				&& line[i + 1] != DOLLAR
				&& line[i + 1] != S_QUOTE
				&& line[i + 1] != D_QUOTE
				&& line[i + 1] != QUESTION)
				break ;
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

static void	replace_dollar(char *str, char **result, int *i, char *q)
{
	t_minishell	*shell;
	char		*tmp;

	shell = get_minishell();
	if (str[*i] == DOLLAR && (str[*i + 1] == D_QUOTE || str[*i + 1] == S_QUOTE))
	{
		if (q[0] || !q[1])
			(*i)++;
		else
		{
			tmp = *result;
			*result = strjoin_char(tmp, str[(*i)++]);
			return ;
		}
	}
	if (str[*i] == DOLLAR && str[(*i) + 1] == QUESTION && str[(*i) + 1])
		get_ext_code(shell, result, i);
	else if (str[*i] == DOLLAR && str[(*i) + 1] != DOLLAR && str[(*i) + 1])
		get_env(shell, result, &str, i);
	else
	{
		tmp = *result;
		*result = strjoin_char(tmp, str[(*i)++]);
	}
}
