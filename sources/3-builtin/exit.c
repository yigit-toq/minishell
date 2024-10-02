/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:46:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/02 17:01:02 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void	check_numeric(char *av, int *exit_code);

int	ft_exit(t_minishell *shell, char **av)
{
	int	exit_code;
	int	arg_count;

	arg_count = 0;
	while (av[arg_count])
		arg_count++;
	ft_dprintf(STD_ERROR, "exit\n");
	if (arg_count > 2)
	{
		ft_dprintf(STD_ERROR, "too many arguments\n");
		exit_code = 1;
	}
	else if (av[1] != NULL)
		check_numeric(av[1], &exit_code);
	else
		exit_code = shell->value.exit_code;
	clear_garbage();
	exit(exit_code);
	return (1);
}

static void	check_numeric(char *av, int *exit_code)
{
	char	*number;
	int		checker;

	checker = 1;
	if (av[0] == '-' || av[0] == '+')
		av++;
	number = ft_itoa(ft_atoi(av));
	if (ft_strcmp(number, av))
		checker = 0;
	gfree(number);
	while (*av)
	{
		if (!ft_isdigit(*av))
			checker = 0;
		av++;
	}
	if (checker)
		(*exit_code) = ft_atoi(av);
	else
	{
		ft_dprintf(STD_ERROR, "minishell: exit: %s", av);
		ft_dprintf(STD_ERROR, ": numeric argument required\n");
		(*exit_code) = 255;
	}
}
