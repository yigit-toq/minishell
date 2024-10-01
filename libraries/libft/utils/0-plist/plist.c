/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:02:04 by ytop              #+#    #+#             */
/*   Updated: 2024/10/01 17:18:38 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_parser	*ft_parser_new(char **args, char **args_with_quote)
{
	t_parser	*new;

	new = ft_calloc(1, sizeof(t_parser));
	if (!new)
		return (NULL);
	new->args_quote = args_with_quote;
	new->args = args;
	new->next = NULL;
	return (new);
}

void	ft_parser_addback(t_parser **lst, t_parser *new)
{
	t_parser	*node;

	node = *lst;
	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

void	ft_parser_delone(t_parser *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->args);
	del(lst->args_quote);
	gfree(lst);
}

void	ft_parser_clear(t_parser **lst, void (*del)(void *))
{
	t_parser	*node;

	if (!lst)
		return ;
	while (*lst)
	{
		node = (*lst)->next;
		ft_parser_delone(*lst, del);
		*lst = node;
	}
	*lst = 0;
}

void	ft_parser_onenode(t_parser *lst, void (*del)(void *))
{
	t_parser	*node;
	if (!lst || !del)
		return ;
	node = lst->next;
	del(lst->args);
	del(lst->args_quote);
	gfree(lst);
	lst = node;
}
