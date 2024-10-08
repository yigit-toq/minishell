/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:16:27 by ytop              #+#    #+#             */
/*   Updated: 2024/10/08 15:24:54 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_to_list(char **env)
{
	t_minishell	*minishell;
	int			i;

	i = 0;
	minishell = get_minishell();
	minishell->env = ft_lstnew(ft_strdup(env[i]));
	while (env[++i])
		ft_lstadd_back(&minishell->env, ft_lstnew(ft_strdup(env[i])));
}

void	get_env(t_minishell *minishell, char **result, char **str, int *i)
{
	t_list		*env_value;
	char		*new_result;
	char		*env_var;
	char		*tmp;
	int			v_st;

	v_st = ++(*i);
	while ((*str)[*i] && ft_isalnum((*str)[*i]))
		(*i)++;
	env_var = ft_substr(*str, v_st, (*i) - v_st);
	env_value = search_env(minishell, env_var);
	gfree(env_var);
	if (!env_value)
		return ;
	tmp = get_value(env_value->content);
	new_result = ft_strjoin(*result, tmp);
	gfree(tmp);
	gfree(*result);
	*result = new_result;
}

void	print_env(void)
{
	t_minishell	*minishell;
	t_list		*env_data;

	minishell = get_minishell();
	env_data = minishell->env;
	while (env_data)
	{
		if (ft_strchr(env_data->content, '='))
			printf("%s\n", env_data->content);
		env_data = env_data->next;
	}
}

int	get_key(char *line)
{
	int	j;

	j = 0;
	while (line[j] && line[j] != '=')
		j++;
	return (j);
}

char	*get_value(char *line)
{
	char	*value;
	int		i;

	if (!line)
		return (NULL);
	i = get_key(line);
	if (!line[i])
		return (NULL);
	value = ft_strdup(line + i + 1);
	return (value);
}
