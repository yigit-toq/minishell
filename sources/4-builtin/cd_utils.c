/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:52:30 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/04 15:25:53 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_oldpwd(t_list *old_pwd_env, t_list *pwd_env, char *val)
{
	if (old_pwd_env && pwd_env)
	{
		val = get_value(pwd_env->content);
		if (val)
		{
			gfree(old_pwd_env->content);
			old_pwd_env->content = ft_strjoin("OLDPWD=", val);
			gfree(val);
		}
	}
}

void	change_pwd(t_minishell *minishell, char *pwd)
{
	t_list	*pwd_env;
	t_list	*old_pwd_env;
	char	*val;

	pwd_env = search_env(minishell, "PWD");
	old_pwd_env = search_env(minishell, "OLDPWD");
	val = NULL;
	change_oldpwd(old_pwd_env, pwd_env, val);
	if (pwd_env)
	{
		if (!old_pwd_env)
			ft_lstadd_back(&minishell->env, ft_lstnew(
					ft_strjoin("OLDPWD=", minishell->value.old_pwd)));
		gfree(pwd_env->content);
		pwd_env->content = ft_strjoin("PWD=", pwd);
	}
}
