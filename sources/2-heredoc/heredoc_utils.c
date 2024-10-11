/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:42:21 by ytop              #+#    #+#             */
/*   Updated: 2024/10/11 14:02:39 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	null_heredoc_args(char **args)
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

static int	delimiter_utils(t_minishell *shell, t_parser *tmp, t_parser *delim)
{
	int	i;
	int	j[2];

	i = 0;
	ft_bzero(j, sizeof(int) * 2);
	while (i <= shell->value.pipe_count)
	{
		shell->value.hrdc_fd[i] = -1;
		if (delim && delim->args[0])
		{
			if (read_heredoc(shell, delim->args, j, i))
				return (FAILURE);
		}
		tmp = tmp->next;
		delim = delim->next;
		i++;
		j[0] = 0;
	}
	return (SUCCESS);
}

int	delimiter(t_minishell *shell)
{
	t_parser	*delim;
	t_parser	*tmp;

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
	if (delimiter_utils(shell, tmp, delim))
		return (FAILURE);
	return (SUCCESS);
}
