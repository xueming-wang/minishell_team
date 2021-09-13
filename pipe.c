/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 13:12:12 by dchheang          #+#    #+#             */
/*   Updated: 2021/09/13 15:15:01 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_ms *ms, int *pipe_fd)
{
	t_cmd cmd;

	cmd = *(t_cmd*)ms->cmd_list_ite->next->content;
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		print_error_msg(strerror(errno), PIPE_ERR, ms);
	close(pipe_fd[0]);
	if (cmd.flag == '|')
		run_pipe(ms);
	else
		run_cmd(&cmd);
	exit(EXIT_SUCCESS);
}

void	exec_parent(t_ms *ms, int *pipe_fd)
{
	int		status;
	t_cmd	cmd;

	cmd = *(t_cmd *)ms->cmd_list_ite->content;
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		print_error_msg(strerror(errno), PIPE_ERR, ms);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	run_cmd(&cmd);
	wait(&status);
}

void	run_pipe(t_ms *ms)
{
	int		pid;
	int		pipe_fd[2];
	t_cmd	current_cmd;
	t_cmd	next_cmd;

	current_cmd = *(t_cmd*)ms->cmd_list_ite->content;
	if (ms->cmd_list_ite->next == NULL)
		print_error_msg("syntax error : expected cmd after '|'", SYNTAX_ERR, ms);
	next_cmd = *(t_cmd*)ms->cmd_list_ite->next->content;
	if (pipe(pipe_fd) == -1)
		print_error_msg(strerror(errno), PIPE_ERR, ms);
	pid = fork();
	if (!pid)
		exec_child(ms, pipe_fd);
	else
		exec_parent(ms, pipe_fd);
}
