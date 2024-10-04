/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:26:37 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/04 14:06:05 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//sadece export yazıldığında olan hata düzeltilecek
// unset ile path çıkarılıp ls atınca seg

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
			if (env[i + 1] == ' ' || (i > 0 && env[i - 1] == ' '))
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
	char	*val;

	if (!args || !args[1])
	{
		new = ft_lstcopy(minishell->env);
		ft_lstsort(&new, ft_strcmp);
		while (new)
		{
			ft_dprintf(STD_OUTPUT, "declare -x %s\n", new->content);
			val = get_value(new->content);
			if (val && ft_printf("=\"") && ft_printf("%s", val))
				ft_printf("\"");
			ft_printf("\n");
			gfree(val);
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
	t_list	*srch;

	srch = search_env(minishell, key);
	if (srch && ft_strncmp(srch->content, arg, ft_strlen(arg)))
	{
		gfree(srch->content);
		srch->content = ft_strdup(arg);
	}
	else if (!srch)
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
