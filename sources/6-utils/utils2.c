/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:39:11 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/08 17:07:20 by abakirca         ###   ########.fr       */
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
	err_msg(NULL, get_value(search_env(minishell, "HOME")->content)
		, "is a directory");
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
