/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchheang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 18:36:52 by dchheang          #+#    #+#             */
/*   Updated: 2021/09/10 18:40:08 by dchheang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_elem_from_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		array[i] = array[i + 1];
		i++;
	}
}
