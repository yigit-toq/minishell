/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 00:19:13 by ytop              #+#    #+#             */
/*   Updated: 2024/10/09 15:03:07 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_token(t_minishell *minishell);
static void	count_heredoc(t_minishell *minishell);

void	lexer(t_minishell *minishell)
{
	if (minishell->token)
		ft_lstclear(&minishell->token, &del);
	create_token(minishell);
	count_heredoc(minishell);
}

static void	process_token(t_minishell *shell, char *line, int last_pipe, int i);

static void	create_token(t_minishell *minishell)
{
	char	*line;
	int		index;
	int		last_pipe;

	line = ft_strtrim(minishell->line, " ");
	index = 0;
	last_pipe = 0;
	if (!line)
		return ;
	while (TRUE)
	{
		if ((line[index] == PIPE || !line[index]) && !check_quote(line, index))
		{
			process_token(minishell, line, last_pipe, index);
			if (line[index] == '\0')
				break ;
			last_pipe = index + 1;
			minishell->value.pipe_count++;
		}
		index++;
	}
	gfree(line);
}

static void	process_token(t_minishell *minishell,
			char *line, int last_pipe, int i)
{
	char	*temp_str;
	char	*temp_line;

	temp_str = ft_substr(line, last_pipe, i - last_pipe);
	temp_line = ft_strtrim(temp_str, " ");
	gfree(temp_str);
	if (!temp_line)
		return ;
	if (!minishell->token)
		minishell->token = ft_lstnew(temp_line);
	else
		ft_lstadd_back(&minishell->token, ft_lstnew(temp_line));
}

static void	count_heredoc(t_minishell *minishell)
{
	char	*line;
	int		i;

	i = 0;
	line = minishell->line;
	minishell->value.hrdc_count = 0;
	while (line[i])
	{
		if (line[i] == INPUT && line[i + 1] == INPUT && !check_quote(line, i))
		{
			minishell->value.hrdc_count++;
			i++;
		}
		i++;
	}
}
