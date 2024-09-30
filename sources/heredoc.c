/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:23:35 by ytop              #+#    #+#             */
/*   Updated: 2024/09/30 17:45:05 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(void)
{
	t_minishell	*minishell;
	t_parser	*parser;

	minishell = get_minishell();
	parser = minishell->parser;
	if (!parser)
		return (SUCCESS);
	while (parser)
	{
		if (heredoc_syntax(parser->args))
			return (minishell->value.exit_code = 2, FAILURE);
		parser = parser->next;
	}
	if (!minishell->value.hrdc_count)
		return (SUCCESS);
	if (delimiter(minishell, parser))
		return (FAILURE);
	if (!minishell->value.pipe_count)
		gfree(minishell->hrdc_fd);
	return (SUCCESS);
}
