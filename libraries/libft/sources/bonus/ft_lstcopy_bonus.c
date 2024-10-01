/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstcopy_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 23:59:26 by ytop              #+#    #+#             */
/*   Updated: 2024/10/01 14:26:00 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstcopy(t_list *lst)
{
	t_list	*new;
	t_list	*node;

	if (!lst)
		return (0);
	new = ft_lstnew(lst->content);
	if (!new)
		return (0);
	node = lst->next;
	while (node)
	{
		node = ft_lstnew(node->content);
		if (!node)
		{
			ft_lstclear(&new, del);
			return (0);
		}
		ft_lstadd_back(&new, node);
		node = node->next;
	}
	return (new);
}

// Güncellenebilir.
