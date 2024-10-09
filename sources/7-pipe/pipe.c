/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:25:42 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:24:27 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

static int	create_pipe(void)
{
	t_minishell	*minishell;
	int			i;

	i = 0;
	minishell = get_minishell();
	minishell->value.pipe_fd = ft_calloc(minishell->value.pipe_count * 2,
			sizeof(int));
	minishell->pid = ft_calloc(minishell->value.pipe_count + 1, sizeof(int));
	if (!minishell->value.pipe_fd || !minishell->pid)
		return (perror("ft_calloc"), FAILURE);
	while (i < minishell->value.pipe_count)
	{
		if (pipe(minishell->value.pipe_fd + i * 2) == -1)
			return (perror("pipe"), FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	close_fd(void)
{
	t_minishell	*minishell;
	int			i;

	i = 0;
	minishell = get_minishell();
	while (i < minishell->value.pipe_count * 2)
	{
		close(minishell->value.pipe_fd[i]);
		i++;
	}
	i = 0;
	while (i < minishell->value.pipe_count + 1)
	{
		waitpid(minishell->pid[i], &minishell->value.exit_code, 0);
		minishell->value.exit_code = WEXITSTATUS(minishell->value.exit_code);
		i++;
	}
	gfree(minishell->pid);
	gfree(minishell->value.pipe_fd);
	return (SUCCESS);
}

int	ft_pipe(char **cmd, t_parser *parser)
{
	t_minishell	*minishell;
	t_parser	*tmp;
	int			i;

	i = 0;
	tmp = parser;
	minishell = get_minishell();
	if (create_pipe())
		return (FAILURE);
	while (i < minishell->value.pipe_count + 1 && tmp)
	{
		minishell->pid[i] = fork();
		if (minishell->pid[i] == -1)
			return (FAILURE);
		else if (minishell->pid[i] == 0)
			pipe_fork(cmd, tmp, i);
		tmp = tmp->next;
		i++;
	}
	if (minishell->value.hrdc_count > 0 && minishell->value.hrdc_fd)
		gfree(minishell->value.hrdc_fd);
	return (close_fd());
}
