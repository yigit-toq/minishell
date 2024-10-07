/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 21:32:09 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 13:48:16 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_word_start(char *str, char delim, int *i, int *q_flag);
static int	count_words(char *str, char delim);

char	**parser_split(char *str, char delimiter)
{
	int		indexes[4];
	char	**result;
	char	*substr;

	ft_bzero(indexes, sizeof(indexes));
	result = ft_calloc(count_words(str, delimiter) + 1, sizeof(char *));
	if (!result)
		return (0);
	while (str[indexes[0]])
	{
		indexes[1] = get_word_start(str, delimiter, &indexes[0], &indexes[3]);
		if (indexes[0] > indexes[1])
		{
			substr = ft_substr(str, indexes[1], indexes[0] - indexes[1]);
			if (substr)
			{
				replace_arg(&substr);
				result[indexes[2]++] = substr;
			}
		}
	}
	result[indexes[2]] = 0;
	return (result);
}

static int	is_quote(char c);

static int	count_words(char *str, char delim)
{
	int	indexes[3];

	ft_bzero(indexes, sizeof(indexes));
	while (*str)
	{
		if (is_quote(*str))
		{
			if (indexes[2] == 0)
				indexes[2] = *str;
			else if (indexes[2] == *str)
				indexes[2] = 0;
		}
		if (*str != delim || indexes[2])
		{
			if (!indexes[1] && indexes[0]++)
				indexes[1] = 1;
		}
		else
			indexes[1] = 0;
		str++;
	}
	return (indexes[0]);
}

static void	update_quote(char current_char, int *quote_flag);

static int	get_word_start(char *str, char delim, int *i, int *q_flag)
{
	int	start;

	while (str[*i] && (str[*i] == delim && !*q_flag))
		(*i)++;
	start = *i;
	while (str[*i] && (str[*i] != delim || *q_flag))
	{
		update_quote(str[*i], q_flag);
		(*i)++;
	}
	return (start);
}

static int	is_quote(char c)
{
	return (c == S_QUOTE || c == D_QUOTE);
}

static void	update_quote(char current_char, int *quote_flag)
{
	if (is_quote(current_char))
	{
		if (*quote_flag == 0)
			*quote_flag = current_char;
		else if (*quote_flag == current_char)
			*quote_flag = 0;
	}
}
