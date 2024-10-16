/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 01:07:10 by ytop              #+#    #+#             */
/*   Updated: 2024/10/11 13:08:14 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static int		process_tokens(t_minishell *minishell, t_parser *parser);

int	parser(t_minishell *minishell)
{
	t_parser	*parser;
	int			t_count;

	t_count = ft_lstsize(minishell->token);
	parser = allocate_parser(t_count);
	if (!parser)
		return (FAILURE);
	if (process_tokens(minishell, parser))
		return (FAILURE);
	minishell->parser = parser;
	return (SUCCESS);
}

t_parser	*allocate_parser(int count)
{
	t_parser	*head;
	t_parser	*new;
	int			i;

	i = 0;
	head = NULL;
	while (i < count)
	{
		new = ft_parser_new(NULL);
		if (!new)
			return (perror("ft_calloc"), NULL);
		ft_parser_addback(&head, new);
		i++;
	}
	return (head);
}

static int	handle_token(t_parser *node, t_list *token)
{
	char	*token_content;

	token_content = ft_strdup(token->content);
	if (!token_content)
		return (perror("ft_strdup"), FAILURE);
	if (replace_arg(&token_content))
		return (FAILURE);
	node->args = parser_split(token_content, SPACE);
	if (!node->args)
		return (perror("ft_split"), FAILURE);
	gfree(token_content);
	return (SUCCESS);
}

static int	process_tokens(t_minishell *minishell, t_parser *parser)
{
	t_parser	*current_parser;
	t_list		*token;

	current_parser = parser;
	token = minishell->token;
	while (current_parser && token)
	{
		if (handle_token(current_parser, token))
			return (FAILURE);
		current_parser = current_parser->next;
		token = token->next;
	}
	return (SUCCESS);
}

void	parser_update(t_minishell *shell, t_parser *tmp, t_parser *head)
{
	t_parser	*new;
	int			index[2];

	head = allocate_parser(ft_lstsize(shell->token));
	new = head;
	while (tmp)
	{
		ft_bzero(index, sizeof(int) * 2);
		new->args = ft_calloc(ft_size(tmp->args) + 1, sizeof(char *));
		while (tmp->args[index[0]])
		{
			if (!ft_strcmp(dollar(0, 0, tmp->args[index[0]]), ""))
			{
				index[0]++;
				continue ;
			}
			new->args[index[1]] = ft_strdup(dollar(0, 0, tmp->args[index[0]]));
			index[0]++;
			index[1]++;
		}
		new = new->next;
		tmp = tmp->next;
	}
	ft_parser_clear(&shell->parser, del);
	shell->parser = head;
}
