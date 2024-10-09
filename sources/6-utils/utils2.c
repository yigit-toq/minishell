/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:39:11 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/09 17:45:11 by abakirca         ###   ########.fr       */
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
