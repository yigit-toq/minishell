/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:16:19 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 15:20:02 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>

void	free_n_null(char **args, int *j)
{
	(*j) += 2;
	gfree(args[(*j)]);
	args[(*j)] = NULL;
	gfree(args[(*j) + 1]);
	args[(*j) + 1] = NULL;
}

static int	redirect_out(char *file, int append)
{
	char	*clean_file;
	int		fd;

	clean_file = handle_quotes(file);
	if (append)
		fd = open(clean_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(clean_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	gfree(clean_file);
	if (fd == -1)
		return (perror("minishell"), FAILURE);
	if (fd >= 0 && dup2(fd, STD_OUTPUT) == -1)
		return (perror("minishell"), close(fd), FAILURE);
	return (close(fd), SUCCESS);
}

static int	redirect_in(char *file)
{
	char	*clean_file;
	int		fd;

	clean_file = handle_quotes(file);
	fd = open(clean_file, O_RDONLY);
	gfree(clean_file);
	if (fd == -1)
		return (perror("minishell"), FAILURE);
	if (fd >= 0 && dup2(fd, STD_INPUT) == -1)
		return (perror("minishell"), close(fd), FAILURE);
	return (close(fd), SUCCESS);
}

static int	find_exec(char **args, char **file, int *i, int *j)
{
	if (!(ft_strcmp(args[(*j)], ">") || !ft_strcmp(args[(*j)], ">>")) && args[(*j) + 1])
	{
		*file = args[(*j) + 1];
		if (ft_strcmp(args[(*j)], ">") == 0)
		{
			if (redirect_out(*file, j, 0))
				return (FAILURE);
		}
		else if (redirect_out(*file, j, 1))
			return (FAILURE);
		free_n_null(args, j);
	}
	else if (!ft_strcmp(args[(*j)], "<") && args[(*j) + 1])
	{
		*file = args[(*j) + 1];
		if (redirect_in(*file, j))
			return (FAILURE);
		free_n_null(args, j);
	}
	else
		args[(*i)++] = args[(*j)++];
	return (SUCCESS);
}

static int	ft_redirect(char **args)
{
	t_minishell	*minishell;
	char		*file;
	int			i;
	int			j;

	i = 0;
	j = 0;
	save_fd();
	minishell = get_minishell();
	while (args[i])
	{
		if (find_exec(args, &file, &i, &j))
			return (FAILURE);
	}
	args[j] = NULL;
	return (SUCCESS);
}
