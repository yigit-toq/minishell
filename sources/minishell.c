/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:41:40 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 19:40:09 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	g_signal;

static int	minishell_loop(t_minishell *minishell, int value);

static void	starting(void);

int	main(int argc, char *argv[], char *env[])
{
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	starting();
	handle_signals();
	disable_echo();
	minishell = get_minishell();
	ft_bzero(minishell, sizeof(t_minishell));
	env_to_list(env);
	minishell_loop(minishell, 0);
	return (SUCCESS);
}

static void	init_data(void);

static int	minishell_routine(t_minishell *minishell);

static int	minishell_loop(t_minishell *minishell, int value)
{
	while (TRUE)
	{
		value = 0;
		init_data();
		minishell->line = addgarbage(readline(GREEN PROMPT RESET));
		if (!minishell->line)
		{
			ft_dprintf(STD_OUTPUT, "exit\n");
			rl_clear_history();
			clear_garbage();
			exit(EXIT_SUCCESS);
		}
		if (ft_strlen(minishell->line))
		{
			value = minishell_routine(minishell);
			if (value == FAILURE)
				return (FAILURE);
			else if (value == 2)
				continue ;
		}
		if (minishell->line)
			gfree(minishell->line);
	}
	return (SUCCESS);
}

static int	minishell_routine(t_minishell *minishell)
{
	add_history(minishell->line);
	if (!check_line())
	{
		dollar(minishell, 0);
		lexer(minishell);
		if (parser(minishell) == FAILURE)
			return (FAILURE);
		if (heredoc())
			return (2);
		execute_command();
		reset_fd();
	}
	return (SUCCESS);
}

static void	init_data(void)
{
	t_minishell	*minishell;
	t_value		*value;

	minishell = get_minishell();
	value = &minishell->value;
	value->sign = 0;
	value->pipe_count = 0;
	value->hrdc_count = 0;
	minishell->fd_hl.change = 0;
}

static void	starting(void)
{
	ft_printf(GREEN "\033[H\033[J");
	ft_printf("   _____   __        __        __            __   __\n");
	usleep(100000);
	ft_printf("  /     \\ |__| ____ |__| _____|  |__   ____ |  | |  |\n");
	usleep(100000);
	ft_printf(" /  \\ /  \\|  |/    \\|  |/  ___/  |  \\_/ __ \\|  | |  |\n");
	usleep(100000);
	ft_printf("/    Y    \\  |   |  \\  |\\___ \\|   Y  \\  ___/|  |_|  |__\n");
	usleep(100000);
	ft_printf("\\____|__  /__|___|  /__/____  >___|  /\\___  >____/____/\n");
	usleep(100000);
	ft_printf("        \\/        \\/        \\/     \\/     \\/\n" RESET);
	usleep(500000);
	ft_printf(BLACK "\nCREATED BY:\n");
	usleep(500000);
	ft_printf("\t\t _________________");
	ft_printf("\n\t\t/\t\t  \\\n\t\t");
	ft_printf("| ytop & abakirca |\n");
	ft_printf("\t\t\\_________________/\n");
	ft_printf("\n\n" RESET);
}
