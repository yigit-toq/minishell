/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:27:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 13:59:48 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	empty_dollar(char *line);

static void	replace_dollar(t_minishell *shell, char **result, int *i, char *q);

void	dollar(t_minishell *minishell, int i)
{
	char	*result;
	char	q[2];

	if (empty_dollar(minishell->line))
		return ;
	ft_bzero(q, 2);
	result = ft_strdup("");
	while (minishell->line[i])
	{
		if (!q[0] && minishell->line[i] == S_QUOTE)
			q[0] = minishell->line[i];
		else if (!q[1] && minishell->line[i] == D_QUOTE)
			q[1] = minishell->line[i];
		else if (q[0] && q[0] == minishell->line[i])
			q[0] = 0;
		else if (q[1] && q[1] == minishell->line[i])
			q[1] = 0;
		if (q[0] || (!q[0] && minishell->line[i] == S_QUOTE))
			result = strjoin_char(result, minishell->line[i++]);
		else
			replace_dollar(minishell, &result, &i, q);
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
	if (str[*i] == DOLLAR && str[*i + 1] == D_QUOTE)
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
