/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:51:54 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/09 15:15:00 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	change_dir(t_minishell *mini, char *target, char *av, char *pwd)
{
	if (chdir(target) == -1)
		return (perror("minishell: cd"), mini->value.exit_code = 1, FAILURE);
	if (!getcwd(pwd, 4096))
		return (err_msg("cd: ", target, "No such file or directory"), FAILURE);
	change_pwd(mini, pwd);
	if (av && !ft_strncmp(av, "-", 1))
		ft_printf("%s\n", pwd);
	if (target != av)
		gfree(target);
	return (SUCCESS);
}

static int	check_meta_chars(char *av, t_minishell *minishell, t_list **path)
{
	if (!av || !ft_strncmp(av, "~", 1))
	{
		if (av && !ft_strncmp(av, "~~", 2))
			return (err_msg("cd: ", av, "No such file or directory"),
				minishell->value.exit_code = 1, FAILURE);
		*path = search_env(minishell, "HOME");
		if (!(*path))
			return (err_msg("cd: ", NULL, "Home not set"),
				minishell->value.exit_code = 1, FAILURE);
	}
	else if (!ft_strncmp(av, "-", 1))
	{
		*path = search_env(minishell, "OLDPWD");
		if (!(*path))
			return (err_msg("cd: ", NULL, "OLDPWD not set"),
				minishell->value.exit_code = 1, FAILURE);
	}
	return (SUCCESS);
}

static int	get_target_dir(t_minishell *minishell, char *av, char **target)
{
	t_list	*path;
	char	*env;

	path = NULL;
	env = NULL;
	if (check_meta_chars(av, minishell, &path))
		return (FAILURE);
	if (path)
	{
		env = get_value(path->content);
		if (!env)
			return (err_msg("cd: ", NULL, " No such file or directory"),
				minishell->value.exit_code = 1, FAILURE);
		*target = env;
	}
	else if (av)
		*target = av;
	return (SUCCESS);
}

int	cd(t_minishell *shell, char *av)
{
	char	*target;
	int		err;
	char	*oldpwd;
	char	pwd[4096];

	if (!getcwd(pwd, 4096))
	{
		if (search_env(shell, "PWD")->content)
			ft_strlcpy(pwd, get_value(search_env(shell, "PWD")->content), 4096);
		else
			return (err_msg("cd: ", NULL, "Getcwd error"), FAILURE);
	}
	oldpwd = ft_strdup(pwd);
	if (!oldpwd)
		return (err_msg("cd: ", NULL, "OLDPWD not set"), FAILURE);
	if (shell->value.old_pwd)
		gfree(shell->value.old_pwd);
	shell->value.old_pwd = oldpwd;
	err = get_target_dir(shell, av, &target);
	if (err)
		return (FAILURE);
	if (change_dir(shell, target, av, pwd))
		return (FAILURE);
	return (SUCCESS);
}
