/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 11:19:47 by dchheang          #+#    #+#             */
/*   Updated: 2021/10/17 09:36:12 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ms	init_shell(char **env)
{
	t_ms	ms;

	ms.fd_in = dup(STDIN_FILENO);
	ms.fd_out = dup(STDOUT_FILENO);
	ms.env_list = NULL;
	ms.env_list = get_env(env, ms.env_list);
	ms.envp = env;
	ms.history = init_history(ms.history);
	ms.cmd_list_head = NULL;
	ms.cmd_list_ite = NULL;
	ms.cmd_ret = -1;
	return (ms);
}

int	run_simple_cmd(t_ms *ms, t_cmd *current_cmd)
{
	int	pid;
	int	ret;

	pid = fork();
	if (!pid)
		exit(run_cmd(ms, current_cmd));
	else
	{
		waitpid(pid, &ret, 0);
		ret = WEXITSTATUS(ret);
	}
	return (ret);
}

int	run_context(t_ms *ms)
{
	t_cmd	*current_cmd;
	int		ret;

	ret = 0;
	while (ms->cmd_list_ite)
	{
		current_cmd = (t_cmd *)ms->cmd_list_ite->content;
		if (current_cmd->in_streams || current_cmd->out_streams)
			ret = redirect(ms, current_cmd);
		if (ret)
			break ;
		if (current_cmd->flag == '|')
			run_pipe(ms);
		else
		{
			if (current_cmd->cmd[0])
				ret = run_cmd(ms, current_cmd);
			ms->cmd_list_ite = ms->cmd_list_ite->next;
		}
	}
	return (ret);
}

int	check_error(t_ms *ms)
{
	t_list	*token_list;
	char	*check;

	if (!ms->rdl)
	{
		rl_clear_history();
		ft_lstclear(&ms->env_list, &free);
		close(ms->history.fd);
		free(ms->history.path);
		print_error_msg("exit", 0, ms);
	}
	token_list = NULL;
	token_list = get_tokens(ms->rdl);
	check = check_grammar(token_list);
	if (check)
	{
		printf("parse error near %s\n", check);
		free(ms->rdl);
		ms->rdl = NULL;
		ft_lstclear(&token_list, &free_token);
		return (1);
	}
	ft_lstclear(&token_list, &free_token);
	return (0);
}

void	run_shell(char **env)
{
	t_ms	ms;

	ms = init_shell(env);
	while (1)
	{
		ms.rdl = readline("prompt> ");
		if (!check_error(&ms))
		{
			ms.cmd_list_head = get_cmds(ms.rdl, ms.env_list, &ms);
			ms.cmd_list_head = get_stream(ms.cmd_list_head);
			ms.cmd_list_ite = ms.cmd_list_head;
			ft_add_history(ms.rdl, ms.history);
			if (ms.cmd_list_ite)
				ms.cmd_ret = run_context(&ms);
			free_memory(&ms);
			reset_fdin_fdout(&ms);
		}
	}
}
