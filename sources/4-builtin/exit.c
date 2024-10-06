/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:46:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 14:03:36 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	is_str_digit(char *str)
{
	if (str[0] == '-' || str[0] == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	max_int(char *str)
{
	char	*number;

	if (str[0] == '-' || str[0] == '+')
		str++;
	number = ft_itoa(ft_atoi(str));
	if (ft_strcmp(number, str) != 0)
		return (gfree(number), 0);
	gfree(number);
	return (1);
}

static void	check_numeric(char *av, int *exit_code)
{
	if (is_str_digit(av) && max_int(av))
		(*exit_code) = ft_atoi(av);
	else
	{
		err_msg("exit: ", av, "numeric argument required");
		(*exit_code) = 255;
	}
}

void	ft_exit(t_minishell *minishell, char **av)
{
	int	exit_code;
	int	i;

	i = 0;
	while (av[i])
		i++;
	exit_code = 0;
	ft_dprintf(STD_OUTPUT, "exit\n");
	if (i > 2)
	{
		ft_dprintf(STD_ERROR, " too many arguments\n");
		exit_code = 1;
	}
	else if (av[1] != NULL)
		check_numeric(av[1], &exit_code);
	else
		exit_code = minishell->value.exit_code;
	clear_garbage();
	exit(exit_code);
}
