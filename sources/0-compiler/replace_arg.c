/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:55:22 by ytop              #+#    #+#             */
/*   Updated: 2024/09/30 15:58:58 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_char(char *in, char *buf, int *i, int *j);

// Yeniden düzenlenebilir

int	replace_arg(char *args)
{
	char	*buffer;
	char	*input;
	int		in_len;
	int		src_index;
	int		dst_index;

	input = args;
	in_len = ft_strlen(input);
	buffer = ft_calloc((in_len * 2) + 1, sizeof(char));
	if (!buffer)
		return (perror("ft_calloc"), FAILURE);
	src_index = 0;
	dst_index = 0;
	while (src_index < in_len)
		process_char(input, buffer, &src_index, &dst_index);
	buffer[dst_index] = '\0';
	return (args = buffer, SUCCESS);
}

// gfree(args); line : 36

static void	process_char(char *in, char *buf, int *i, int *j)
{
	if ((in[*i] == REDIRECT || in[*i] == INPUT) && !check_quote(in, *i))
	{
		if (in[*i - 1] != SPACE && in[*i - 1] != REDIRECT && in[*i - 1] != INPUT && *i > 0)
			buf[(*j)++] = SPACE;
		if (in[*i] == REDIRECT && in[*i + 1] == REDIRECT)
		{
			buf[(*j)++] = REDIRECT;
			buf[(*j)++] = REDIRECT;
			(*i) += 2;
		}
		else if (in[*i] == INPUT && in[*i + 1] == INPUT)
		{
			buf[(*j)++] = INPUT;
			buf[(*j)++] = INPUT;
			(*i) += 2;
		}
		else
			buf[(*j)++] = in[(*i)++];
		if (in[*i] != SPACE && in[*i] != REDIRECT && in[*i] != INPUT && *i < (int)ft_strlen(in))
			buf[(*j)++] = SPACE;
	}
	else
		buf[(*j)++] = in[(*i)++];
}
