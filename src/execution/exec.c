/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuwang <xuwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 19:42:50 by dchheang          #+#    #+#             */
/*   Updated: 2021/10/13 10:45:54 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_chdir(DIR *dir, char *file_name)
{
	struct dirent	*dirent;

	dirent = readdir(dir);
	while (dirent != NULL)
	{
		if (!ft_strcmp(dirent->d_name, file_name))
			return (1);
		dirent = readdir(dir);
	}
	closedir(dir);
	return (0);
}

char	*get_exec_path(t_ms *ms, char *cmd)
{
	int		i;
	char	*path;
	char	*tmp;
	char	**split;
	DIR		*dir;

	i = 0;
	path = get_var(ms->env_list, "PATH");
	split = ft_split(path, ":");
	while (split[i])
	{
		dir = opendir(split[i]);
		if (dir && ft_chdir(dir, cmd))
		{
			tmp = ft_strjoin("/", cmd);
			path = ft_strjoin(split[i], tmp);
			free(tmp);
			free_array(split);
			return (path);
		}
		i++;
	}
	return (NULL);
}

int	ft_execve(t_ms *ms, char *path, char **argv)
{
	int		pid;
	int		signal;

	pid = fork();
	if (pid == -1)
		print_error_msg(strerror(errno), errno, ms);
	if (!pid)
	{
		if (execve(path, argv, ms->envp) == -1)
			perror(path);
		exit(errno);
	}
	else
	{
		waitpid(pid, &signal, 0);
		signal = WEXITSTATUS(signal);
	}
	return (signal);
}

int	run_exec(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	int		ret;

	if (ft_strchr(cmd->cmd[0], '/'))
		ret = ft_execve(ms, cmd->cmd[0], cmd->cmd);
	else
	{
		path = get_exec_path(ms, cmd->cmd[0]);
		if (path)
			ret = ft_execve(ms, path, cmd->cmd);
		else
		{
			printf("minishell: %s: command not found\n", cmd->cmd[0]);
			ret = 127;
		}
	}
	return (ret);
}