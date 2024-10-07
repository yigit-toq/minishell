/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_syntax.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 01:12:16 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 20:47:05 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_syntax(char **args);

int	heredoc_syntax(char **args)
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
	if (ft_strcmp(arg, "<>") == 0 || ft_strcmp(arg, "<<<") == 0)
	{
		err_msg(SYNTAX_ERR, "newline'", NULL);
		return (FAILURE);
	}
	if (ft_strcmp(arg, ">>>") == 0)
	{
		err_msg(SYNTAX_ERR, ">'", NULL);
		return (FAILURE);
	}
	if (ft_strcmp(arg, "><") == 0)
	{
		err_msg(SYNTAX_ERR, "<'", NULL);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	check_redirection_error(char **args, int i)
{
	if ((!args[i + 1] || !ft_strlen(args[i + 1]))
		&& (!ft_strcmp(args[i], ">") || !ft_strcmp(args[i], "<")
			|| !ft_strcmp(args[i], ">>") || !ft_strcmp(args[i], "<<")))
	{
		err_msg(SYNTAX_ERR, "newline'", NULL);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	check_syntax(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (check_invalid_operator(args[i]))
			return (FAILURE);
		if (check_redirection_error(args, i))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
