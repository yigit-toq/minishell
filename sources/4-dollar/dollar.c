/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:27:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 17:30:27 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	empty_dollar(char *line);

static void	replace_dollar(t_minishell *shell, char **result,
				int *i, char d_q, char s_q);

void	dollar(t_minishell *shell)
{
	char	*result;
	char	s_q;
	char	d_q;
	int		i;

	if (empty_dollar(shell->line))
		return ;
	i = 0;
	s_q = 0;
	d_q = 0;
	result = ft_strdup("");
	while (shell->line[i])
	{
		if (!s_q && shell->line[i] == S_QUOTE)
			s_q = shell->line[i];
		else if (!d_q && shell->line[i] == D_QUOTE)
			d_q = shell->line[i];
		else if (s_q && s_q == shell->line[i])
			s_q = 0;
		else if (d_q && d_q == shell->line[i])
			d_q = 0;
		if (s_q || (!s_q && shell->line[i] == S_QUOTE))
			result = strjoin_char(result, shell->line[i++]);
		else
			replace_dollar(shell, &result, &i, d_q, s_q);
	}
	gfree(shell->line);
	shell->line = result;
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

static void	replace_dollar(t_minishell *shell, char **result,
			int *i, char d_q, char s_q)
{
	char	*tmp;
	char	*str;

	(void)d_q;
	str = shell->line;
	if (str[*i] == DOLLAR && str[*i + 1] == D_QUOTE)
	{
		if (s_q || !d_q)
			(*i)++;
		else
		{
			tmp = *result;
			*result = strjoin_char(tmp, str[(*i)++]);
			return ;
		}
	}
	if (str[*i] == DOLLAR && str[(*i) + 1] && str[(*i) + 1] == QUESTION)
		get_ext_code(shell, result, i);
	else if (str[*i] == DOLLAR && str[(*i) + 1] && str[(*i) + 1] != DOLLAR)
		get_env(shell, result, &str, i);
	else
	{
		tmp = *result;
		*result = strjoin_char(tmp, str[(*i)++]);
	}
}
