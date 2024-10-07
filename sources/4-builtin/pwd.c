/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:40:41 by ytop              #+#    #+#             */
/*   Updated: 2024/10/07 17:22:24 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

char	*get_pwd(void)
{
	t_minishell	*minishell;
	char		*path;
	char		pwd[4096];

	minishell = get_minishell();
	path = getcwd(pwd, 4096);
	if (!path)
	{
		if (search_env(minishell, "PWD")->content)
			path = get_value(search_env(minishell, "PWD")->content);
		else
			return (perror("minishell: pwd: "), NULL);
	}
	return (ft_printf("%s\n", path), path);
}
