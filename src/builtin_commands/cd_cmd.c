/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 20:05:02 by asabani           #+#    #+#             */
/*   Updated: 2022/02/23 23:28:06 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *cmd, char *arg)
{
	if (arg[0] == '-')
	{
		if (arg[1] == '\0')
			return (1);
		else
			return (cmd_error(cmd, arg, "invalid option"), -1);
	}
	return (0);
}

static void	change_dir(char *next_path, t_venv **venv)
{
	char	*current_path;

	current_path = ft_getcwd();
	if (chdir(next_path) == -1)
		cmd_error("cd", next_path, strerror(errno));
	else
	{
		if (ft_memcmp(next_path, ".", sizeof(".")) == 0 && errno == ENOENT)
			return (cmd_error("cd", "error retrieving current directory: \
getcwd: cannot access parent directories", strerror(errno)));
		venv_insert(venv, "OLDPWD", current_path, E_GLOBAL);
		venv_insert(venv, "PWD", ft_getcwd(), E_GLOBAL);
		g_global.status = set_status(0);
	}
}

void	cd(char **av, t_venv **venv)
{
	int		opt;
	char	*var;

	if (av[0] == NULL)
	{
		var = getvenv("HOME");
		if (!var)
			return (cmd_error("cd", "HOME not set", NULL));
		return (change_dir(var, venv));
	}
	opt = check_option("cd", av[0]);
	if (opt == 1)
	{
		var = getvenv("OLDPWD");
		if (!var)
			return (cmd_error("cd", "OLDPWD not set", NULL));
		return (change_dir(var, venv));
	}
	else if (opt == 0)
		return (change_dir(av[0], venv));
	return ;
}
