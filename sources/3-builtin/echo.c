/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 23:34:21 by ytop              #+#    #+#             */
/*   Updated: 2024/09/30 16:01:54 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(char **args)
{
	int	flag_index;
	int	newline;
	int	index;

	index = 1;
	newline = 1;
	while (args[index][0] == '-' && args[index][1] == 'n' && args[index])
	{
		flag_index = 1;
		while (args[index][flag_index] == 'n')
			flag_index++;
		if (args[index][flag_index] != '\0')
			break ;
		newline = 0;
		index++;
	}
	while (args[index])
	{
		ft_dprintf(STD_OUTPUT, "%s", args[index]);
		if (args[index + 1])
			ft_dprintf(STD_OUTPUT, " ");
		index++;
	}
	if (newline)
		ft_dprintf(STD_OUTPUT, "\n");
}
