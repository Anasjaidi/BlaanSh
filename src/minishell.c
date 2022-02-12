/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/09 02:52:04 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//* names of tokens (for debuging)
const char *token_names[] = {
	"WHITESPACE",
	"TK_NEWLINE",
	"WORD",
	"PATH",
	"PIPE",
	"AND_IF",
	"OR_IF",
	"O_PARENTHESESE",
	"C_PARENTHESESE",
	"EQUAL",
	"AMPERSAND",
	"SEMICL",
	"DSEMICL",
	"VAR_EXPANSION",
	"TILDE_EXPANSION",
	"WILDCARD_EXPANSION",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"LESS",
	"DLESS",
	"GREAT",
	"DGREAT"
};

t_global	g_global = {.gc = 0, \
						.program_name = NULL, \
						.status = 0, \
						.is_running = 0};

int	main(int ac, char **av, char **env)
{
	char	*cmdline;
	t_venv	*venv;
	t_list	*tokens;

	(void)ac;
	(void)av;
	(void)env;
	g_global.gc = gc_init();
	if (!g_global.gc)
	set_program_name(av[0]);
	venv = venv_init(env);
	while (true)
	{
		term_init();
		if (WIFSIGNALED(g_global.status))
			printf("\n");
		cmdline = readline("minishell$ ");
		term_restore();
		if (!cmdline)
			break ;
		gc_append(g_global.gc, cmdline, GC_TMP);
		add_history(cmdline);

		//* debug: show tokens
		tokens = tokenize(cmdline);
		t_node	*top = tokens->top;
		while (top)
		{
			printf("tok = %-40s val = %-20s len = %zu\n", token_names[top->token], top->val, top->val? ft_strlen(top->val): 0);
			top = top->next;
		}
		// list_clear(tokens);
		gc_clean(&g_global.gc, GC_TMP);
		//* end debug
	}
	gc_clean(&g_global.gc, GC_DESTROY_SELF);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
