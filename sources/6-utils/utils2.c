/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:39:11 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/11 17:09:17 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_head(t_list **head)
{
	if (head == NULL || *head == NULL)
		return ;
	*head = (*head)->next;
	if (*head != NULL)
		(*head)->prev = NULL;
}

int	ft_size(char **args)
{
	int		i;
	int		size;
	char	*tmp;

	i = 0;
	size = 0;
	if (!args)
		return (i);
	while (args[i])
	{
		tmp = dollar(0, 0, args[i]);
		if (ft_strcmp(tmp, ""))
			size++;
		i++;
	}
	return (size);
}

static void	handle_redirection(char **args, int *i, char **cmd, int *j)
{
	if (args[*i] && (!ft_strcmp(args[*i], ">>") || !ft_strcmp(args[*i], ">")
			|| !ft_strcmp(args[*i], "<") || !ft_strcmp(args[*i], "<<")))
	{
		*i += 2;
		if (args[*i] == NULL)
			cmd[(*j)++] = ft_strdup("0\0");
		else
			(*i)--;
	}
}

static void	process_args(t_parser *parser_tmp, char **cmd, int *j)
{
	int	i;

	i = -1;
	while (i < ft_size(parser_tmp->args) && parser_tmp->args[++i])
	{
		if (parser_tmp->args[i]
			&& (!ft_strcmp(parser_tmp->args[i], ">>")
				|| !ft_strcmp(parser_tmp->args[i], ">")
				|| !ft_strcmp(parser_tmp->args[i], "<")
				|| !ft_strcmp(parser_tmp->args[i], "<<")))
		{
			handle_redirection(parser_tmp->args, &i, cmd, j);
			continue ;
		}
		if (parser_tmp->args[i])
			cmd[(*j)++] = parser_tmp->args[i];
		break ;
	}
	null_heredoc_args(parser_tmp->args);
}

void	init_cmd(t_parser *parser, char **cmd)
{
	t_parser	*parser_tmp;
	int			j;

	j = 0;
	parser_tmp = parser;
	while (parser_tmp)
	{
		process_args(parser_tmp, cmd, &j);
		parser_tmp = parser_tmp->next;
	}
}
