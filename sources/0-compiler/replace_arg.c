/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:55:22 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 15:21:45 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_char(char *in, char *buf, int *i, int *j);

int	replace_arg(char **args)
{
	char	*buffer;
	char	*input;
	int		in_len;
	int		src_index;
	int		dst_index;

	input = *args;
	in_len = ft_strlen(input);
	buffer = ft_calloc((in_len * 2) + 1, sizeof(char));
	if (!buffer)
		return (perror("ft_calloc"), FAILURE);
	src_index = 0;
	dst_index = 0;
	while (src_index < in_len)
		process_char(input, buffer, &src_index, &dst_index);
	buffer[dst_index] = '\0';
	gfree(*args);
	return (*args = buffer, SUCCESS);
}

static void	process_char(char *in, char *buf, int *i, int *j)
{
	if ((in[(*i)] == '>' || in[(*i)] == '<') && check_quote(in, *i) == 0)
	{
		if ((*i) > 0 && in[(*i) - 1] != ' '
			&& in[(*i) - 1] != '>' && in[(*i) - 1] != '<')
			buf[(*j)++] = ' ';
		if (in[(*i)] == '>' && in[(*i) + 1] == '>')
		{
			buf[(*j)++] = '>';
			buf[(*j)++] = '>';
			(*i) += 2;
		}
		else if (in[(*i)] == '<' && in[(*i) + 1] == '<')
		{
			buf[(*j)++] = '<';
			buf[(*j)++] = '<';
			(*i) += 2;
		}
		else
			buf[(*j)++] = in[(*i)++];
		if ((*i) < (int)ft_strlen(in) && in[(*i)] != ' '
			&& in[(*i)] != '>' && in[(*i)] != '<')
			buf[(*j)++] = ' ';
	}
	else
		buf[(*j)++] = in[(*i)++];
}
