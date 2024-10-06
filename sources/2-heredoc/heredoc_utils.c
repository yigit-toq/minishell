/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 00:59:25 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 00:59:25 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	process_delimiter(t_minishell *minishell, t_parser *delimiter)
{
	t_parser	*parser;
	t_parser	*tmp;
	int			i;

	i = 0;
	tmp = delimiter;
	parser = minishell->parser;
	while (i <= minishell->value.pipe_count && parser && tmp)
	{
		tmp->args = get_delimiters(minishell, parser->args);
		if (!tmp->args && minishell->value.hrdc_count > 0)
			return (ft_parser_clear(&delimiter, del), FAILURE);
		parser = parser->next;
		tmp = tmp->next;
		i++;
	}
	return (SUCCESS);
}

int	delimiter(t_minishell *shell)
{
	t_parser	*parser_tmp;
	t_parser	*delimiter;
	int			i;

	parser_tmp = shell->parser;
	shell->value.hrdc_fd = ft_calloc(shell->value.pipe_count + 1, sizeof(int));
	if (!shell->value.hrdc_fd)
		return (FAILURE);
	delimiter = allocate_parser(shell->value.pipe_count + 1);
	if (!delimiter)
		return (gfree(shell->value.hrdc_fd), FAILURE);
	if (process_delimiter(shell, delimiter) || !delimiter)
	{
		ft_dprintf(STD_ERROR, "%s%s'newline'\n", ERR_TITLE, SYNTAX_ERR);
		shell->value.exit_code = 2;
		return (gfree(shell->value.hrdc_fd), gfree(delimiter), FAILURE);
	}
	i = 0;
	while (i <= shell->value.pipe_count && delimiter)
	{
		shell->value.hrdc_fd[i] = -1;
		if (delimiter && delimiter->args)
			if (read_heredoc(shell, delimiter->args, i))
				return (FAILURE);
		null_heredoc_args(parser_tmp->args);
		parser_tmp = parser_tmp->next;
		delimiter = delimiter->next;
		i++;
	}
	return (SUCCESS);
}
