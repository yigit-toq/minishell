/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 18:16:27 by ytop              #+#    #+#             */
/*   Updated: 2024/10/04 14:05:41 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_to_list(char *env[])
{
	t_minishell	*minishell;
	int			i;

	i = 0;
	minishell = get_minishell();
	minishell->env = ft_lstnew(ft_strdup(env[i]));
	while (env[++i])
	{
		ft_lstadd_back(&minishell->env, ft_lstnew(ft_strdup(env[i])));
	}
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
	while (env_data && ft_printf("%s\n", env_data->content))
		env_data = env_data->next;
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

t_list	*search_env(t_minishell *minishell, char *key)
{
	t_list	*tmp_env;
	char	*env_key;

	if (key == NULL || ft_strlen(key) == 0)
		return (NULL);
	tmp_env = minishell->env;
	while (tmp_env)
	{
		env_key = ft_substr(tmp_env->content, 0, get_key(tmp_env->content));
		if (!ft_strcmp(env_key, key))
		{
			gfree(env_key);
			return (tmp_env);
		}
		gfree(env_key);
		tmp_env = tmp_env->next;
	}
	return (NULL);
}
