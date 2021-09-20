/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 18:36:52 by dchheang          #+#    #+#             */
/*   Updated: 2021/09/20 16:11:07 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	remove_elem_from_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		array[i] = array[i + 1];
		printf("array[%d] = %s\n", i, array[i]);
		i++;
	}
	free(array[i]);
}

void	remove_from_list(t_list **head, t_list *elem)
{
	t_list	*ite;
	t_list	*tmp;

	ite = *head;
	while (ite && ite->next != elem)
		ite = ite->next;
	if (!ite)
		return ;
	tmp = ite->next->next;
	ft_lstdelone(ite->next, &free_cmd);
	ite->next = tmp;
}
