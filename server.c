/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsouquie <lsouquie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 13:20:19 by lsouquie          #+#    #+#             */
/*   Updated: 2023/05/18 19:00:29 by lsouquie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/*faire ;alloc taill liste recopier liste dans truc*/

void	stash(char c, t_lst **lst)
{
	t_lst	*new;

	new = ft_newlst(c);
	ft_add_back(lst, new);
	if (!c)
	{
		print_lst(*lst);
		ft_clear(lst, &free);
	}
}

void	bin_to_char(int signum, char *c, int pid)
{
	if (signum == SIGUSR1)
		*c = (*c << 1) | 1;
	else if (signum == SIGUSR2)
		*c <<= 1;
}

void	serv_sig_handler(int signum, siginfo_t *info, void *content)
{
	static char					c;
	static int					byte_count;
	static int					pid;
	static t_lst				*lst;

	(void)content;
	pid = info->si_pid;
	bin_to_char(signum, &c, pid);
	byte_count++;
	if (byte_count == 8)
	{
		byte_count = 0;

		if (!c)
			kill(pid, SIGUSR1);
		stash(c, &lst);
		c = 0;
	}
	kill(pid, SIGUSR2);
}

int	main(void)
{
	struct sigaction	sa;
	int					pid;

	pid = getpid();
	ft_printf("%d\n", pid);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = serv_sig_handler;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (1);
}
