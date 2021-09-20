/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:50:32 by dchheang          #+#    #+#             */
/*   Updated: 2021/09/20 19:10:04 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_fdin_fdout(t_ms *ms)
{
	dup2(ms->fd_in, STDIN_FILENO);
	dup2(ms->fd_out, STDOUT_FILENO);
	close(ms->fd_in);
	close(ms->fd_out);
}

void	redirect_in_out(t_ms *ms, int newfd, int flags)
{
	int		fd;
	t_cmd	next_cmd;

	if (ms->cmd_list_ite->next == NULL)
		print_error_msg("missing argument after redirection\n", SYNTAX_ERR, ms);
	next_cmd = *(t_cmd *)ms->cmd_list_ite->next->content;
	if (flags & O_CREAT)
		fd = open(next_cmd.cmd[0], flags, 0666);
	else
		fd = open(next_cmd.cmd[0], flags);
	if (fd == -1)
		print_error_msg(strerror(errno), FILE_ERR, ms);
	if (dup2(fd, newfd) == -1)
		print_error_msg(strerror(errno), PIPE_ERR, ms);
	close(fd);
}

void	read_from_current_input(t_ms *ms)
{
	int		rd;
	int		fd;
	char	buff[BUFFER_SIZE];

	if (ms->cmd_list_ite->next == NULL)
		print_error_msg("missing argument after redirection\n", SYNTAX_ERR, ms);
	next_cmd = *(t_cmd *)ms->cmd_list_ite->next->content;
	fd = open("/tmp/heredoc.txt", O_RDWR | O_APPEND | O_CREAT, 0666);
	rd = read(STDIN_FILENO, buff, BUFFER_SIZE - 1);
	while (ft_strncmp(buff, next_cmd.cmd[0], rd - 1) && rd != -1)
	{
		buff[rd] = 0;
		write(fd, buff, rd);
		rd = read(STDIN_FILENO, buff, BUFFER_SIZE - 1);
	}
	if (rd == -1)
		print_error_msg(strerror(errno), READ_WRITE_ERR, ms);
	close(fd);
	return (line);
}

void	redirect(t_ms *ms, t_cmd *current_cmd)
{
	t_cmd	*tmp;
	t_cmd	next_cmd;
	char	*line;

	if (current_cmd->flag == SLR)
		redirect_in_out(ms, STDIN_FILENO, O_RDWR);
	else if (current_cmd->flag == SRR)
		redirect_in_out(ms, STDOUT_FILENO, O_RDWR | O_CREAT | O_TRUNC);
	else if (current_cmd->flag == DRR)
		redirect_in_out(ms, STDOUT_FILENO, O_RDWR | O_CREAT | O_APPEND);
	else
		line = read_from_current_input(ms);
	if (!current_cmd->cmd[0])
	{
		printf("cc = %s\n", current_cmd->cmd[0]);
		ms->cmd_list_ite = ms->cmd_list_ite->next;
		*current_cmd = *(t_cmd *)ms->cmd_list_ite->content;
		printf("next cmd = %s\n", current_cmd->cmd[0]);
		remove_elem_from_array(current_cmd->cmd);
		printf("next cmd = %s\n", current_cmd->cmd[0]);
		if (!current_cmd->cmd[0] && ms->cmd_list_ite->next)
		{
			ms->cmd_list_ite = ms->cmd_list_ite->next;
			*current_cmd = *(t_cmd *)ms->cmd_list_ite->content;
		}
	}
	else
	{
		next_cmd = *(t_cmd *)ms->cmd_list_ite->next->content;
		current_cmd->flag = next_cmd.flag;
		remove_from_list(&ms->cmd_list_head, ms->cmd_list_ite->next);
	}
}
