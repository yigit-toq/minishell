/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 00:18:53 by ytop              #+#    #+#             */
/*   Updated: 2024/10/01 16:22:59 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	err_msg(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd(ERR_TITLE, STD_ERROR);
	if (cmd)
		ft_dprintf(STD_ERROR, "%s", cmd);
	if (arg)
		ft_dprintf(STD_ERROR, "`%s'", arg);
	if (msg)
		ft_dprintf(STD_ERROR, ": %s", msg);
	ft_dprintf(STD_ERROR, "\n");
	return (SUCCESS);
}

void	save_fd(void)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	if (!minishell->fd_hl.change)
	{
		minishell->fd_hl.change = TRUE;
		minishell->fd_hl.std_in = dup(STD_INPUT);
		minishell->fd_hl.std_out = dup(STD_OUTPUT);
	}
}

void	reset_fd(void)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	if (minishell->fd_hl.change)
	{
		minishell->fd_hl.change = 0;
		dup2(minishell->fd_hl.std_in, STD_INPUT);
		dup2(minishell->fd_hl.std_out, STD_OUTPUT);
	}
}
