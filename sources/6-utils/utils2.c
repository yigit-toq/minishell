/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:39:11 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/09 15:21:53 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*get_minishell(void)
{
	static t_minishell	minishell;

	return (&minishell);
}

void	get_home(void)
{
	t_minishell	*minishell;

	minishell = get_minishell();
	err_msg(NULL, get_value(search_env(minishell, "HOME")->content),
		"is a directory");
	minishell->value.exit_code = 126;
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
