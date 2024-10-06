/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 23:56:12 by ytop              #+#    #+#             */
/*   Updated: 2024/10/06 23:56:12 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes_helper(char **tmp1, char **tmp2, char **new_tmp)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while ((*tmp2)[i])
	{
		if (quote == 0 && ((*tmp2)[i] == S_QUOTE || (*tmp2)[i] == D_QUOTE))
			quote = (*tmp2)[i];
		else if (quote != 0 && quote == (*tmp2)[i])
			quote = 0;
		else
		{
			*new_tmp = strjoin_char(*tmp1, (*tmp2)[i]);
			if (!(*new_tmp))
				return ;
			*tmp1 = *new_tmp;
		}
		i++;
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
			remove_quotes_helper(&tmp_one, &tmp_two, &new_tmp);
			parser_tmp->args[i] = tmp_one;
			i++;
		}
		parser_tmp = parser_tmp->next;
	}
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
