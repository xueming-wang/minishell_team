/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 10:07:07 by dchheang          #+#    #+#             */
/*   Updated: 2021/10/21 14:49:41 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_error_fd(t_ms *ms)
{
	int	error_fd;

	error_fd = ft_open("tmp/error_file.txt", O_RDWR | O_TRUNC | O_CREAT, 0666, ms);
	ft_dup2(error_fd, STDERR_FILENO, ms);
	close(error_fd);
}

void	read_error(t_ms *ms)
{
	char	buff[BUFFER_SIZE];
	int		rd;
	int		fd;

	fd = ft_open("tmp/error_file.txt", O_RDONLY, 0, ms);
	rd = read(fd, buff, BUFFER_SIZE - 1);
	while (rd > 0)
	{
		write(STDERR_FILENO, buff, rd);
		rd = read(fd, buff, BUFFER_SIZE - 1);
	}
	close(fd);
}