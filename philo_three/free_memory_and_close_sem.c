/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory_and_close_sem.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 09:39:37 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void		free_memory_and_close_sem(t_params *params)
{
	free(params->child_pids);
	sem_close(params->sem_forks);
	sem_close(params->sem_died);
	sem_close(params->sem_params);
	sem_close(params->sem_exit_code);
	sem_close(params->sem_both_forks);
}
