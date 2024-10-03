/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:25:42 by ytop              #+#    #+#             */
/*   Updated: 2024/10/03 14:14:58 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void	handle_heredoc_dup(int i)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	if (minishell->value.hrdc_count && minishell->value.hrdc_fd[i] != -1)
	{
		if (dup2(minishell->value.hrdc_fd[i], STD_INPUT) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(minishell->value.hrdc_fd[i]);
	}
	else if (i != 0)
	{
		if (dup2(minishell->value.pipe_fd[(i - 1) * 2], STD_INPUT) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

static void	handle_pipe_dup(int i)
{
	t_minishell	*minishell;
	int			j;

	j = 0;
	minishell = get_minishell();
	handle_heredoc_dup(i);
	if (i != minishell->value.pipe_count)
	{
		if (dup2(minishell->value.pipe_fd[i * 2 + 1], STD_OUTPUT) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
	while (j < minishell->value.pipe_count * 2)
	{
		close(minishell->value.pipe_fd[j]);
		j++;
	}
}

static int	create_pipe(void)
{
	t_minishell	*minishell;
	int			i;

	i = 0;
	minishell = get_minishell();
	minishell->value.pipe_fd = ft_calloc(minishell->value.pipe_count * 2, sizeof(int));
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

static void	fork_execute(t_parser *parser, char **cmd, int i)
{
	t_minishell	*minishell;
	char		**environment;

	minishell = get_minishell();
	environment = env();
	minishell->path = find_path(cmd[i]);
	if (execve(minishell->path, parser->args, environment) == -1)
	{
		type_control(minishell->parser, environment);
		err_msg(NULL, parser->args[0], ": command not found");
		if (minishell->value.sign)
			gfree(minishell->path);
		exit(127);
	}
	if (minishell->value.sign)
		gfree(minishell->path);
}

static void	pipe_fork(char **cmd, t_parser *parser, int i)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	handle_pipe_dup(i);
	if (check_redirect(parser->args))
		exit(FAILURE);
	ft_all_lower(&cmd[i]);
	if (check_builtin(minishell, cmd, parser))
		exit(minishell->value.exit_code);
	fork_execute(parser, cmd, i);
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
		else if (!minishell->pid[i])
			pipe_fork(cmd, tmp, i);
		tmp = tmp->next;
		i++;
	}
	if (minishell->value.hrdc_count > 0 && minishell->value.hrdc_fd)
		gfree(minishell->value.hrdc_fd);
	return (close_fd());
}
