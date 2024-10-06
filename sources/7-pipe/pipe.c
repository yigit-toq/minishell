/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:25:42 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 15:24:05 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	close_fd(void)
{
	t_minishell	*minishell;
	int			index;

	index = 0;
	minishell = get_minishell();
	while (index < minishell->value.pipe_count * 2)
	{
		close(minishell->value.pipe_fd[index]);
		index++;
	}
	index = 0;
	while (index < minishell->value.pipe_count + 1)
	{
		waitpid(minishell->pid[index], &minishell->value.exit_code, 0);
		minishell->value.exit_code = WEXITSTATUS(minishell->value.exit_code);
		index++;
	}
	gfree(minishell->pid);
	gfree(minishell->value.pipe_fd);
	return (SUCCESS);
}

static int	create_pipe(void)
{
	t_minishell	*minishell;
	int			*pipe_fd;
	int			index;

	minishell = get_minishell();
	index = 0;
	minishell->pid = ft_calloc(minishell->value.pipe_count + 1, sizeof(int));
	pipe_fd = ft_calloc(minishell->value.pipe_count * 2, sizeof(int));
	if (!minishell->pid || !pipe_fd)
		return (perror("ft_calloc"), FAILURE);
	while (index < minishell->value.pipe_count)
	{
		if (pipe(pipe_fd + index * 2) == -1)
			return (perror("pipe"), FAILURE);
		index++;
	}
	minishell->value.pipe_fd = pipe_fd;
	return (SUCCESS);
}

int	ft_pipe(char **cmd, t_parser *parser)
{
	t_minishell	*minishell;
	t_parser	*pars_tmp;
	int			i;

	i = 0;
	pars_tmp = parser;
	minishell = get_minishell();
	if (create_pipe())
		return (FAILURE);
	while (i < minishell->value.pipe_count + 1 && pars_tmp)
	{
		minishell->pid[i] = fork();
		if (minishell->pid[i] == -1)
			return (FAILURE);
		else if (minishell->pid[i] == 0)
			pipe_fork(cmd, pars_tmp, i);
		pars_tmp = pars_tmp->next;
		i++;
	}
	if (minishell->value.hrdc_fd && minishell->value.hrdc_count > 0)
		gfree(minishell->value.hrdc_fd);
	return (close_fd());
}
