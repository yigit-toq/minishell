/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:27:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/08 14:10:05 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	empty_dollar(char *line);

static void	replace_dollar(t_minishell *shell, char **result, int *i, char *q);

static void dollar_ext(t_minishell *minishell, int i, char *q)
{
	if (!q[0] && minishell->line[i] == S_QUOTE)
		q[0] = minishell->line[i];
	else if (!q[1] && minishell->line[i] == D_QUOTE)
		q[1] = minishell->line[i];
	else if (q[0] && q[0] == minishell->line[i])
		q[0] = 0;
	else if (q[1] && q[1] == minishell->line[i])
		q[1] = 0;
}

void	dollar(t_minishell *minishell, int i, int j)
{
	char	*result;
	char	q[2];

	if (empty_dollar(minishell->line))
		return ;
	ft_bzero(q, 2);
	result = ft_strdup("");
	while (minishell->line[i])
	{
		dollar_ext(minishell, i, q);
		if ((q[0] || (!q[0] && minishell->line[i] == S_QUOTE)) && !j)
		{
			if (q[1])
			{
				j = 1;
				continue;
			}
			result = strjoin_char(result, minishell->line[i++]);
		}
		else
			replace_dollar(minishell, &result, &i, q);
		j = 0;
	}
	gfree(minishell->line);
	minishell->line = result;
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

static void	replace_dollar(t_minishell *shell, char **result, int *i, char *q)
{
	char	*tmp;
	char	*str;

	str = shell->line;
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
