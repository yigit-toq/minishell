/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:23:35 by ytop              #+#    #+#             */
/*   Updated: 2024/10/11 16:26:15 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>

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
		if (minishell->value.signal == TRUE)
		{
			if (!minishell->value.pipe_count)
				reset_fd();
			return (free(line), FAILURE);
		}
		if (!line || ft_strcmp(line, delim[*j]) == 0)
		{
			free(line);
			j[0] += 1;
			break ;
		}
		if (j[1] == minishell->value.hrdc_count - 1)
			ft_dprintf(fd[1], "%s\n", dollar(0, 0, line));
		free(line);
	}
	return (SUCCESS);
}

int	read_heredoc(t_minishell *shell, char **delimiter, int *j, int i)
{
	int		fd[2];

	if (pipe(fd) == -1)
		return (perror("pipe"), FAILURE);
	if (!shell->value.pipe_count)
		save_fd();
	shell->value.signal = 2;
	while (delimiter[j[0]] && shell->value.signal == 2)
	{
		if (heredoc_loop(shell, delimiter, fd, j))
			return (FAILURE);
		j[1] += 1;
	}
	close(fd[1]);
	if (shell->value.pipe_count)
		shell->value.hrdc_fd[i] = fd[0];
	else
	{
		dup2(fd[0], STD_INPUT);
		close(fd[0]);
	}
	return (SUCCESS);
}
