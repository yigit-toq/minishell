/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:21:07 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 14:36:13 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_parser
{
	char			**args;
	struct s_parser	*next;
}					t_parser;

t_parser	*ft_parser_new(char **args);

void		ft_parser_clear(t_parser **lst, void (*del)(void *));

void		ft_parser_addback(t_parser **lst, t_parser *new);
#endif
