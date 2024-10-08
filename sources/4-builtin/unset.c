/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 23:56:50 by ytop              #+#    #+#             */
/*   Updated: 2024/10/08 16:49:45 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_key(char *key);

void	unset(t_minishell *shell, char **keys)
{
	t_list	*env_node;
	int		key_index;

	key_index = 0;
	while (keys[++key_index])
	{
		if (is_valid_key(keys[key_index]) == FAILURE)
		{
			err_msg("unset: ", keys[key_index], "not a valid identifier");
			shell->value.exit_code = 1;
			continue ;
		}
		if (!ft_strcmp(keys[key_index], "_"))
			continue ;
		env_node = search_env(shell, keys[key_index]);
		if (env_node)
		{
			if (key_index == 1)
				free_head(&shell->env);
			else
				ft_lstdelone(env_node, del);
		}
	}
}

static int	is_valid_key(char *key)
{
	int	key_index;

	key_index = 0;
	if (ft_isdigit(key[0]))
		return (FAILURE);
	while (key[key_index])
	{
		if (!ft_isalnum(key[key_index]))
			return (FAILURE);
		key_index++;
	}
	return (SUCCESS);
}
