/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:16:19 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:14:14 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void	free_n_null(char **args, int *j)
{
	gfree(args[(*j)]);
	gfree(args[(*j) + 1]);
	args[(*j)] = NULL;
	args[(*j) + 1] = NULL;
	(*j) += 2;
}

static int	rdirect_out(char *file, int append)
{
	int		fd;
	char	*clean_file;

	clean_file = handle_quotes(file);
	if (append)
		fd = open(clean_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(clean_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	gfree(clean_file);
	if (fd == -1)
	{
		perror("minishell");
		return (1);
	}
	if (fd >= 0 && dup2(fd, STD_OUTPUT) == -1)
		return (perror("dup2"), close(fd), 1);
	return (close(fd), 0);
}

static int	rdirect_in(char *file)
{
	int			fd;
	char		*clean_file;
	t_minishell	*minishell;

	minishell = get_minishell();
	clean_file = handle_quotes(file);
	fd = open(clean_file, O_RDONLY);
	gfree(clean_file);
	if (fd == -1)
	{
		perror("minishell");
		return (1);
	}
	if (minishell->value.hrdc_count)
		return (close(fd), 0);
	else if (fd >= 0 && dup2(fd, STD_INPUT) == -1)
		return (perror("minishell"), close(fd), 1);
	return (close(fd), 0);
}

int	find_exec(char **args, int *j, int *i, char **file)
{
	if ((ft_strcmp(args[(*j)], ">") == 0 && args[(*j) + 1])
		|| (ft_strcmp(args[(*j)], ">>") == 0 && args[(*j) + 1]))
	{
		*file = args[(*j) + 1];
		if (ft_strcmp(args[(*j)], ">") == 0)
		{
			if (rdirect_out(*file, 0))
				return (FAILURE);
		}
		else if (rdirect_out(*file, 1))
			return (FAILURE);
		free_n_null(args, j);
	}
	else if (ft_strcmp(args[(*j)], "<") == 0 && args[(*j) + 1])
	{
		*file = args[(*j) + 1];
		if (rdirect_in(*file))
			return (FAILURE);
		free_n_null(args, j);
	}
	else
		args[(*i)++] = args[(*j)++];
	return (SUCCESS);
}

int	ft_redirect(char **args)
{
	char		*file;
	int			i;
	int			j;

	i = 0;
	j = 0;
	save_fd();
	while (args[i])
	{
		if (find_exec(args, &i, &j, &file))
			return (FAILURE);
	}
	args[j] = NULL;
	return (SUCCESS);
}
