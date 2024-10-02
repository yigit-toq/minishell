/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:51:54 by abakirca          #+#    #+#             */
/*   Updated: 2024/10/02 18:04:40 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	change_dir(t_minishell *mini, char *target, char *av, char *pwd)
{
	if (chdir(target) == -1)
		return (perror("minishell: cd"), mini->value.exit_code = 1, FAILURE);
	if (!getcwd(pwd, 4096))
		return (err_msg("cd: ", NULL, "Getcwd error"), FAILURE);
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
			return (err_msg("cd: ", av, " No such file or directory"),
				minishell->value.exit_code = 1, FAILURE);
		*path = search_env(minishell, "HOME");
		if (!(*path))
			return (err_msg("cd: ", NULL, " HOME not set"),
				minishell->value.exit_code = 1, FAILURE);
	}
	else if (!ft_strncmp(av, "-", 1))
	{
		*path = search_env(minishell, "OLDPWD");
		if (!(*path))
			return (err_msg("cd: ", NULL, " OLDPWD not set"),
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

int	cd(t_minishell *minishell, char *av)
{
	char	*target;
	int		err;
	char	*oldpwd;
	char	pwd[4096];

	if (!getcwd(pwd, 4096))
		return (err_msg("cd: ", NULL, "Getcwd error"), FAILURE);
	oldpwd = ft_strdup(pwd);
	if (!oldpwd)
		return (err_msg("cd: ", NULL, "OLDPWD not set"), FAILURE);
	if (minishell->value.old_pwd)
		gfree(minishell->value.old_pwd);
	minishell->value.old_pwd = oldpwd;
	err = get_target_dir(minishell, av, &target);
	if (err)
		return (FAILURE);
	if (change_dir(minishell, target, av, pwd))
		return (FAILURE);
	return (SUCCESS);
}
