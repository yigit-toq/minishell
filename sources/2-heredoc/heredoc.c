/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:23:35 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 17:44:00 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	read_heredoc(t_minishell *minishell, char **delimiter, int i);

int	heredoc(void)
{
	t_minishell	*minishell;
	t_parser	*parser;

	minishell = get_minishell();
	parser = minishell->parser;
	if (!parser)
		return (SUCCESS);
	while (parser)
	{
		if (heredoc_syntax(parser->args))
			return (minishell->value.exit_code = 2, FAILURE);
		parser = parser->next;
	}
	if (!minishell->value.hrdc_count)
		return (SUCCESS);
	if (delimiter(minishell))
		return (FAILURE);
	if (!minishell->value.pipe_count)
		gfree(minishell->value.hrdc_fd);
	return (SUCCESS);
}

static int	heredoc_loop(t_minishell *minishell, char **delim, int *fd, int *j)
{
	char	*line;

	while (TRUE)
	{
		line = readline("> ");
		if (g_signal == TRUE)
		{
			if (!minishell->value.pipe_count)
				reset_fd();
			return (free(line), FAILURE);
		}
		if (!line || ft_strcmp(line, delim[*j]) == 0)
		{
			free(line);
			*j += 1;
			break ;
		}
		ft_dprintf(fd[1], "%s\n", line);
		free(line);
	}
	return (SUCCESS);
}

static int	read_heredoc(t_minishell *minishell, char **delimiter, int i)
{
	int		fd[2];
	int		j;

	if (pipe(fd) == -1)
		return (perror("pipe"), FAILURE);
	if (!minishell->value.pipe_count)
		save_fd();
	j = 0;
	g_signal = 2;
	while (delimiter[j] && g_signal == 2)
	{
		if (heredoc_loop(minishell, delimiter, fd, &j))
			return (FAILURE);
	}
	close(fd[1]);
	if (minishell->value.pipe_count)
	{
		minishell->value.hrdc_fd[i] = fd[0];
	}
	else
	{
		dup2(fd[0], STD_INPUT);
		close(fd[0]);
	}
	return (SUCCESS);
}
