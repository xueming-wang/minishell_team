/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuwang <xuwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 17:55:53 by xuwang            #+#    #+#             */
/*   Updated: 2021/09/26 18:27:48 by xuwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int		is_flag(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_quot quote_init(void) {
    t_quot quote_info;
    
    quote_info.quot = NO_Q;
    quote_info.quot_status = STATUS_CLOSE;
    return quote_info;
}
int check_flag(char *cmd, int i)
{
	t_quot quot= quote_init();
	while(cmd[i])
	{
		if (cmd[i] == '\'')
		{
			if (quot.quot == IS_DQ)
            {
                ++i;
                continue;
            }
            quot.quot = IS_SQ;
            if (quot.quot_status == STATUS_CLOSE) {
                quot.quot_status = STATUS_OPEN;
			}
			else {
                quot.quot_status = STATUS_CLOSE;
                quot.quot = NO_Q;
			}
		}
		else if (cmd[i] == '"')
		{
			if (quot.quot == IS_SQ)
            {
                ++i;
                continue;
            }
            quot.quot = IS_DQ;
            if (quot.quot_status == STATUS_CLOSE) {
                quot.quot_status = STATUS_OPEN;
			}
			else {
                quot.quot_status = STATUS_CLOSE;
                quot.quot = NO_Q;
			}
		}
		if (is_flag(cmd[i]) && quot.quot_status == STATUS_CLOSE)
			return (i);
		i++;
	}
	return (i);
}


int		get_flag(char *s)
{
	if (!is_flag(*s))
		return (0);
	if (*s == '<' && (*(s + 1) == '<'))
		return (DLR);
	if (*s == '>' && (*(s + 1) == '<'))
		return (DRR);
	return (*s);
}

int		check_rdl(t_ms *ms)
{
	if (ms->rdl[0] == '|')
	{
		printf("parsing error\n");
		free_memory(ms);
		return (0);
	}
	if (!ms->cmd_list_ite)
		return (0);
	return (1);
}
t_cmd	*init_cmd(void)
{
	t_cmd *cmd = NULL;

	cmd = malloc(sizeof(t_cmd));
	if (cmd){
		cmd->cmd = NULL;
		cmd->flag = 0;
	}
	return (cmd);
}
t_list	*get_cmds(char *s)
{
	t_list	*cmd_list;
	t_cmd	*ctmp;
	char	*tmp;
	int		start;
	int		i;

	i = 0;
	cmd_list = NULL;
	ctmp = NULL;
	while (s[i])
	{
		start = i;
		i = check_flag(s, i);
		// while (s[i] && !is_flag(s[i]))
		// 	i++;
		tmp = ft_substr(s, start, i - start); //截断一个pipe 获取里面的cmd 放进数组里
		ctmp = init_cmd();
		ctmp->cmd = lst_to_tab(tmp);//j ai change
		ctmp->flag = get_flag(&s[i]);
		free(tmp);
		tmp = NULL;
		if (ctmp->flag == DLR || ctmp->flag == DRR)
			i++;
		
		ft_lstadd_back(&cmd_list, ft_lstnew(ctmp));
		i++;
	}
	return (cmd_list);
}
