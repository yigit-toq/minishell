/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:23:35 by ytop              #+#    #+#             */
/*   Updated: 2024/10/02 18:57:15 by abakirca         ###   ########.fr       */
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

// geçici : reset_fd() : 39

static char	**get_delimiters(t_minishell *minishell, char **args)
{
	char	**delimiters;
	int		i;
	int		j;

	i = 0;
	j = 0;
	delimiters = ft_calloc(minishell->value.hrdc_count + 1, sizeof(char *));
	if (!delimiters)
		return (NULL);
	while (args[i])
	{
		if (ft_strcmp(args[i], "<<") == 0 && args[i + 1])
		{
			delimiters[j] = ft_strdup(args[i + 1]);
			if (!delimiters[j])
			{
				while (--j >= 0)
					gfree(delimiters[j]);
				return (gfree(delimiters), NULL);
			}
			j++;
		}
		i++;
	}
	return (delimiters);
}

// delimiters[j] = NULL; : 69

static int	process_delimiter(t_minishell *minishell, t_parser *delim)
{
	t_parser	*parser;
	t_parser	*tmp;
	int			i;

	i = 0;
	tmp = delim;
	parser = minishell->parser;
	while (i <= minishell->value.pipe_count && parser && tmp)
	{
		tmp->args = get_delimiters(minishell, parser->args);
		if (!tmp->args && minishell->value.hrdc_count > 0)
			return (ft_parser_clear(&delim, del), FAILURE);
		parser = parser->next;
		tmp = tmp->next;
		i++;
	}
	return (SUCCESS);
}

static void	null_heredoc_args(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (!ft_strncmp(args[i], "<<", 2) && args[i + 1])
		{
			gfree(args[i]);
			gfree(args[i + 1]);
			i += 2;
		}
		else
		{
			args[j] = args[i];
			i++;
			j++;
		}
	}
	args[j] = NULL;
}

int	delimiter(t_minishell *shell)
{
	t_parser	*delim;
	t_parser	*tmp;
	int			i;

	tmp = shell->parser;
	shell->value.hrdc_fd = ft_calloc(shell->value.pipe_count + 1, sizeof(int));
	if (!shell->value.hrdc_fd)
		return (FAILURE);
	delim = allocate_parser(shell->value.pipe_count + 1);
	if (!delim)
		return (gfree(shell->value.hrdc_fd), FAILURE);
	if (process_delimiter(shell, delim) || !delim)
	{
		ft_dprintf(STD_ERROR, "%s%s 'newline'\n", ERR_TITLE, SYNTAX_ERR);
		shell->value.exit_code = 2;
		return (gfree(shell->value.hrdc_fd), gfree(delim), FAILURE);
	}
	i = 0;
	while (i <= shell->value.pipe_count && delim)
	{
		shell->value.hrdc_fd[i] = -1;
		if (delim && delim->args)
		{
			if (read_heredoc(shell, delim->args, i))
				return (ft_parser_clear(&delim, del), FAILURE);
		}
		null_heredoc_args(tmp->args);
		tmp = tmp->next;
		delim = delim->next;
		i++;
	}
	return (SUCCESS);
}

// gfree(delim) : 118

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
