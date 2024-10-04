/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 00:32:47 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 15:20:25 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	single_command(t_minishell *minishell);
static int	multiple_command(void);

int	execute_command(void)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	if (!minishell->value.pipe_count)
		return (single_command(minishell));
	else
		return (multiple_command());
	return (FAILURE);
}

int			create_fork(char **cmd, t_parser *parser);

static int	single_command(t_minishell *minishell)
{
	t_parser	*parser;
	char		**cmd;
	int			i;

	i = 0;
	parser = minishell->parser;
	if (check_redirect(parser->args))
		return (FAILURE);
	if (parser->args[0] == NULL)
		return (reset_fd(), SUCCESS);
	cmd = ft_calloc(ft_lstsize(minishell->token) + 1, sizeof(char *));
	if (!cmd)
		return (FAILURE);
	remove_quotes(parser);
	init_cmd(parser, cmd);
	ft_all_lower(&cmd[0]);
	if (check_builtin(minishell, cmd, parser, &i))
		return (SUCCESS);
	if (create_fork(cmd, parser))
		return (FAILURE);
	return (SUCCESS);
}

static int	multiple_command(void)
{
	t_minishell	*minishell;
	t_parser	*parser;
	char		**cmd;

	minishell = get_minishell();
	parser = minishell->parser;
	cmd = ft_calloc(ft_lstsize(minishell->token) + 1, sizeof(char *));
	if (!cmd)
		return (FAILURE);
	remove_quotes(parser);
	init_cmd(parser, cmd);
	if (ft_pipe(cmd, parser))
		return (FAILURE);
	return (SUCCESS);
}

int	create_fork(char **cmd, t_parser *parser)
{
	t_minishell	*minishell;
	pid_t		pid;

	minishell = get_minishell();
	if (ft_strncmp(cmd[0], "./", 2))
		minishell->path = find_path(cmd[0]);
	else
		minishell->path = cmd[0];
	g_signal = 3;
	pid = fork();
	if (pid < 0)
	{
		if (minishell->value.sign)
			gfree(minishell->path);
		return (perror("fork"), FAILURE);
	}
	check_pid(parser, &pid);
	if (minishell->value.sign)
		gfree(minishell->path);
	g_signal = 0;
	return (SUCCESS);
}
