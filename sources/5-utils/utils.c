/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 00:18:53 by ytop              #+#    #+#             */
/*   Updated: 2024/09/30 16:11:25 by ytop             ###   ########.fr       */
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
