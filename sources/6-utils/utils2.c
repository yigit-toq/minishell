/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:39:11 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/10 20:10:49 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*get_minishell(void)
{
	static t_minishell	minishell;

	return (&minishell);
}

void	free_head(t_list **head)
{
	t_list	*temp;

	if (head == NULL || *head == NULL)
		return ;
	temp = *head;
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

void	init_cmd_mltpl(t_parser *parser, char **cmd)
{
	t_parser	*parser_tmp;
	int			i;
	int			j;

	j = 0;
	parser_tmp = parser;
	while (parser_tmp)
	{
		i = -1;
		while (parser_tmp->args[++i])
		{
			if (!ft_strcmp(parser_tmp->args[i], ">>") || !ft_strcmp(parser_tmp->args[i], ">") || !ft_strcmp(parser_tmp->args[i], "<") || !ft_strcmp(parser_tmp->args[i], "<<"))
			{
				i += 2;
				if (parser_tmp->args[i] == NULL)
				{
					cmd[j++] = NULL;
					continue;
				}
				else
				{
					i -= 1;
					continue;
				}
			}
			else
			{
				cmd[j++] = ft_strdup(parser_tmp->args[i]);
				break;
			}
		}
		null_heredoc_args(parser_tmp->args);
			parser_tmp = parser_tmp->next;
	}
}
