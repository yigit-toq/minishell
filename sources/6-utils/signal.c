/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:54:23 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:20:48 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>
#include <stdio.h>
#include <readline/readline.h>
#include <sys/ioctl.h>

static void	ctrl_c(int signal);
static void	ctrl_bs(int signal);

void	disable_echo(void)
{
	struct termios	tty;

	if (!tcgetattr(STD_INPUT, &tty))
	{
		tty.c_lflag &= ~ECHOCTL;
		tcsetattr(STD_INPUT, TCSANOW, &tty);
	}
}

void	handle_signals(void)
{
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, ctrl_bs);
}

static void	ctrl_c_utils(void)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	minishell->value.exit_code = 1;
}

static void	ctrl_c(int signal)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	(void)signal;
	if (g_signal == 2)
	{
		ft_printf("\033[A");
		ioctl(0, TIOCSTI, "\n");
	}
	else if (g_signal == 3)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		minishell->value.exit_code = 130;
		g_signal = 0;
		return ;
	}
	else
	{
		ctrl_c_utils();
	}
	g_signal = 1;
}

static void	ctrl_bs(int signal)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	(void)signal;
	if (g_signal == 3)
	{
		ft_printf("Quit: 3\n");
		rl_on_new_line();
		ft_printf("\033[K");
		minishell->value.exit_code = 131;
		g_signal = 4;
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
}
