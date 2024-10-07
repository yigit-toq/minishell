/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:43:25 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 14:13:46 by ytop             ###   ########.fr       */
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
		err_msg(NULL, parser->args[0], "command not found");
		if (minishell->value.sign)
			gfree(minishell->path);
		exit(127);
	}
	if (minishell->value.sign)
		gfree(minishell->path);
}

void	pipe_fork(char **cmd, t_parser *parser, int i)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	handle_pipe_dup(i);
	if (check_redirect(parser->args))
		exit(FAILURE);
	ft_all_lower(&cmd[i]);
	if (check_builtin(cmd, parser, &i))
		exit(minishell->value.exit_code);
	fork_execute(parser, cmd, i);
}
