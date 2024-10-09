/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:39:53 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:19:04 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
