/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:27:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/02 16:54:18 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	empty_dollar(char *line);

static void	replace_dollar(t_minishell *shell, char **result, int *i);

void	dollar(t_minishell *shell)
{
	char	*result;
	char	quote;
	int		i;

	if (empty_dollar(shell->line))
		return ;
	i = 0;
	quote = 0;
	result = ft_strdup("");
	while (shell->line[i])
	{
		if (!quote && (shell->line[i] == S_QUOTE || shell->line[i] == D_QUOTE))
			quote = shell->line[i];
		else if (quote && quote == shell->line[i])
			quote = 0;
		if (quote == S_QUOTE || (!quote && shell->line[i] == S_QUOTE))
			result = strjoin_char(result, shell->line[i++]);
		else
			replace_dollar(shell, &result, &i);
	}
	gfree(shell->line);
	shell->line = result;
}

// Düzenleme yapılabilir

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

// Düzenleme yapılabilir

static void	replace_dollar(t_minishell *shell, char **result, int *i)
{
	char	*tmp;
	char	*str;

	str = shell->line;
	if (str[*i] == DOLLAR && str[*i + 1] == D_QUOTE)
	{
		tmp = *result;
		*result = strjoin_char(tmp, str[(*i)++]);
	}
	else if (str[*i] == DOLLAR && str[(*i) + 1] && str[(*i) + 1] == QUESTION)
		get_ext_code(shell, result, i);
	else if (str[*i] == DOLLAR && str[(*i) + 1] && str[(*i) + 1] != DOLLAR)
		get_env(shell, result, &str, i);
	else
	{
		tmp = *result;
		*result = strjoin_char(tmp, str[(*i)++]);
	}
}
