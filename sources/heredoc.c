/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:23:35 by ytop              #+#    #+#             */
/*   Updated: 2024/09/30 17:45:05 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_syntax(char **args);

int	heredoc(void)
{
	t_minishell	*minishell;
	t_parser	*parser;
	int			i;

	minishell = get_minishell();
	parser = minishell->parser;
	i = 0;
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
	if (delimiter(minishell, parser))
		return (FAILURE);
	if (!minishell->value.pipe_count)
		gfree(minishell->hrdc_fd);
	return (SUCCESS);
}

static int	check_syntax(char **args);

static int	heredoc_syntax(char **args)
{
	int	len;
	int	i;

	if (check_syntax(args))
		return (FAILURE);
	i = 0;
	while (args[i])
	{
		len = ft_strlen(args[i]);
		if (len >= 2 && !ft_strncmp(args[i], "<<", 2))
		{
			if (len > 2)
				return (err_msg(SYNTAX_ERR, "`<<'", NULL), 1);
			if (args[i + 1] && ft_strncmp(args[i + 1], ">>", 2) == 0)
				return (err_msg(SYNTAX_ERR, "`>>'", NULL), 1);
			if (args[i + 1] && ft_strncmp(args[i + 1], "<<", 2) == 0)
				return (err_msg(SYNTAX_ERR, "`<<'", NULL), 1);
		}
		i++;
	}
	return (SUCCESS);
}

static int	check_invalid_operator(char *arg)
{
	if (ft_strcmp(arg, "<>") == 0 || ft_strcmp(arg, "><") == 0)
	{
		err_msg(SYNTAX_ERR, "newline'", NULL);
		return (1);
	}
	if (ft_strcmp(arg, ">>>") == 0 || ft_strcmp(arg, "<<<") == 0)
	{
		err_msg(SYNTAX_ERR, ">'", NULL);
		return (1);
	}
	return (0);
}

static int	check_redirection_error(char **args, int i)
{
	if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], "<") == 0
			|| ft_strcmp(args[i], ">>") == 0 || ft_strcmp(args[i], "<<") == 0)
		&& (!args[i + 1] || ft_strlen(args[i + 1]) == 0))
	{
		err_msg(SYNTAX_ERR, "newline'", NULL);
		return (1);
	}
	return (0);
}

static int	check_syntax(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (check_invalid_operator(args[i]))
			return (1);
		if (check_redirection_error(args, i))
			return (1);
		i++;
	}
	return (0);
}
