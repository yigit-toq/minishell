/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:46:30 by ytop              #+#    #+#             */
/*   Updated: 2024/10/03 16:23:03 by abakirca         ###   ########.fr       */
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
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(av, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		(*exit_code) = 255;
	}
}

int	ft_exit(t_minishell *shell, char **av)
{
	int	exit_code;
	int	i;

	i = 0;
	while (av[i])
		i++;
	exit_code = 0;
	ft_putstr_fd("exit\n", STD_OUTPUT);
	if (i > 2)
	{
		ft_putstr_fd(" too many arguments\n", STDERR_FILENO);
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
