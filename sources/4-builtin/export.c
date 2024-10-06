/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:26:37 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/04 15:36:37 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_check(char *env)
{
	int	i;

	i = 0;
	if (ft_isdigit(env[0]) || env[0] == '=')
		return (FAILURE);
	while (env[i])
	{
		if (env[i] == '=')
		{
			if (env[i + 1] == SPACE || (i > 0 && env[i - 1] == SPACE))
				return (FAILURE);
			break ;
		}
		if (!(ft_isalnum(env[i]) || env[i] == '_'))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

static int	export_no_args(char **args, t_minishell *minishell)
{
	t_list	*new;
	t_list	*tmp;
	char	*value;

	if (!args || !args[1])
	{
		new = ft_lstcopy(minishell->env);
		ft_lstsort(&new, ft_strcmp);
		while (new)
		{
			ft_printf("declare -x %s", new->content);
			value = get_value(new->content);
			if (value && ft_printf("=\"") && ft_printf("%s", value))
				ft_printf("\"");
			ft_printf("\n");
			gfree(value);
			tmp = new;
			new = new->next;
			gfree(tmp);
		}
		return (FAILURE);
	}
	return (SUCCESS);
}

static void	search_n_add(t_minishell *minishell, char *arg, char *key)
{
	t_list	*search;

	search = search_env(minishell, key);
	if (search && ft_strncmp(search->content, arg, ft_strlen(arg)))
	{
		gfree(search->content);
		search->content = ft_strdup(arg);
	}
	else if (!search)
		ft_lstadd_back(&minishell->env, ft_lstnew(ft_strdup(arg)));
}

void	export(t_minishell *minishell, char **args)
{
	char	*key;
	int		i;

	minishell->value.exit_code = 0;
	if (export_no_args(args, minishell))
		return ;
	i = 0;
	while (args[++i])
	{
		if (arg_check(args[1]))
		{
			err_msg("export", args[i], "not a valid identifier");
			minishell->value.exit_code = 1;
			continue ;
		}
		key = ft_substr(args[i], 0, get_key(args[i]));
		if (!ft_strcmp(key, "_"))
		{
			gfree(key);
			continue ;
		}
		search_n_add(minishell, args[i], key);
		gfree(key);
	}
}
