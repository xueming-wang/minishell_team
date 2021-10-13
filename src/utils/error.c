/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:21:15 by dchheang          #+#    #+#             */
/*   Updated: 2021/10/06 19:53:54 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(void *content)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = (t_cmd *)content;
	while (cmd->cmd[i])
	{
		free(cmd->cmd[i]);
		i++;
	}
	free(cmd->cmd);
	free(cmd);
}

void	free_memory(t_ms *ms)
{
	free(ms->rdl);
	ms->rdl = NULL;
	ft_lstclear(&ms->cmd_list_head, &free_cmd);
}

void	print_error_msg(char *s, int error_id, t_ms *ms)
{
	printf("%s\n", s);
	free_memory(ms);
	exit(error_id);
}