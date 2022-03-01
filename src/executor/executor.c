/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:12:41 by asabani           #+#    #+#             */
/*   Updated: 2022/03/02 00:25:01 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define READ_END	0
#define WRITE_END	1
#define LEFT_SIDE	1
#define RIGHT_SIDE	2

pid_t	run_pipe(t_connector *connector, int fds[2], int side)
{
	pid_t		pid;
	int			end;
	int			fileno;
	t_cmdtree	*cmdtree;

	end = WRITE_END;
	fileno = STDOUT_FILENO;
	cmdtree = connector->lcmdtree;
	if (side & RIGHT_SIDE)
	{
		end = READ_END;
		fileno = STDIN_FILENO;
		cmdtree = connector->rcmdtree;
	}
	pid = ft_fork();
	if (pid == 0)
	{
		ft_dup2(fds[end], fileno);
		close_pipe(fds);
		executor(cmdtree);
		check_status();
		exit(get_status());
	}
	return (pid);
}

void	run_pipeline(t_connector *connector)
{
	int	status;
	int	pids[2];
	int	fds[2];

	if (pipe(fds) == -1)
		return (perror("pipe"));
	if (run_pipe(connector, fds, LEFT_SIDE) == -1)
		return ;
	if (run_pipe(connector, fds, RIGHT_SIDE) == -1)
		return ;
	close_pipe(fds);
	if (wait(&status) == pids[1])
		g_global.status = status;
	if (wait(&status) == pids[1])
		g_global.status = status;
}

void	run_cmdlist(t_cmdlist *cmdlist)
{
	char	**argv;

	argv = list_export_array(cmdlist->cmdvec);
	exec_cmd(argv[0], argv);
}

void	executor(t_cmdtree *tree)
{
	if (!tree)
		return ;
	if (tree->node_type == NODE_CMDLST)
		return (run_cmdlist((t_cmdlist *)tree));
	else if (tree->node_type == NODE_PIPE)
		return (run_pipeline((t_connector *)tree));
}