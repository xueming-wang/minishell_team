/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_quot3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuwang <xuwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 15:28:14 by xuwang            #+#    #+#             */
/*   Updated: 2021/10/19 14:08:23 by xuwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_merge(t_list *list1)
{
	char	*merge_cmd;

	merge_cmd = NULL;
	while (list1)
	{
		if (((t_cmdinfo *)list1->content)->status == TO_MERGE)
			merge_cmd = ft_join(merge_cmd, ((t_cmdinfo *)list1->content)->cmd);
		else
			break ;
		list1 = list1->next;
	}
	merge_cmd = ft_join(merge_cmd, ((t_cmdinfo *)list1->content)->cmd);
	return (merge_cmd);
}

t_list	*new_list(t_list *cmd)
{
	t_list	*new_list;
	char	*new_cmd;

	new_list = NULL;
	new_cmd = NULL;
	while (cmd)
	{
		if (((t_cmdinfo *)cmd->content)->status == TO_MERGE)
		{
			new_cmd = cmd_merge(cmd);
			while (((t_cmdinfo *)cmd->content)->status == TO_MERGE)
				cmd = cmd->next;
			cmd = cmd->next;
			ft_lstadd_back(&new_list, ft_lstnew((void *)new_cmd));
		}
		else
		{
			new_cmd = ((t_cmdinfo *)cmd->content)->cmd;
			ft_lstadd_back(&new_list, ft_lstnew((void *)new_cmd));
			cmd = cmd->next;
		}
	}
	return (new_list);
}

char	**lst_to_tab(char *cmd, t_list *env_list, t_ms *ms)
{
	// char	**cmds;
	// int		len;
	// int		i;
	// t_list	*tmp;
	// t_list	*list_after_parsing;
	t_lsttab lsttab;
	
	ft_bzero(&lsttab, sizeof(t_lsttab));
	lsttab.cmds = NULL;
	lsttab.len = 0;
	lsttab.i = 0;
	lsttab.tmp = NULL;
	lsttab.list_after_parsing = NULL;
	lsttab.tmp = sepa_cmd(cmd, env_list, ms);
	lsttab.list_after_parsing = new_list(lsttab.tmp);
	lsttab.len = ft_lstsize(lsttab.list_after_parsing);
	lsttab.cmds = malloc(sizeof(char *) * (lsttab.len + 1));
	while (lsttab.list_after_parsing && lsttab.i < lsttab.len)
	{
		lsttab.cmds[lsttab.i] = lsttab.list_after_parsing->content;
		lsttab.list_after_parsing = lsttab.list_after_parsing -> next;
		lsttab.i++;
	}
	lsttab.cmds[lsttab.i] = NULL;
	return (lsttab.cmds);
}
