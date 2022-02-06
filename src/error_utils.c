/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:20:51 by asabani           #+#    #+#             */
/*   Updated: 2022/02/06 00:35:22 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_error(char *msg)
{
	perror(msg);
	gc_clean(&g_global.gc, GC_ALL);
	exit(EXIT_FAILURE);
}

void	alloc_error(void)
{
	errno = ENOMEM;
	exit_with_error("malloc");
}
