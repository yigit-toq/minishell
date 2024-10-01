/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:21:07 by ytop              #+#    #+#             */
/*   Updated: 2024/10/01 17:18:30 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_parser
{
	char			**args_quote;
	char			**args;
	struct s_parser	*next;
}					t_parser;

t_parser	*ft_parser_new(char **args, char **args_with_quotes);

void		ft_parser_addback(t_parser **lst, t_parser *new);

void		ft_parser_clear(t_parser **lst, void (*del)(void *));

void		ft_parser_onenode(t_parser *lst, void (*del)(void *));
#endif
