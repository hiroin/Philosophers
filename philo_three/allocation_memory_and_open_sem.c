/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation_memory_and_open_sem.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 08:20:01 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

bool		allocation_memory_and_open_sem(t_params *params)
{
	if ((params->child_pids =
			malloc(sizeof(pid_t *) * params->number_of_phs)) == NULL)
		return (ERROR);
	if ((params->sem_forks = sem_open(\
			SEMNAME_FORK, O_CREAT, 0600, params->number_of_phs)) == SEM_FAILED)
		return (ERROR);
	if ((params->sem_died = sem_open(\
			SEMNAME_DIED, O_CREAT, 0600, 1)) == SEM_FAILED)
		return (ERROR);
	if ((params->sem_params = sem_open(\
			SEMNAME_PARAMS, O_CREAT, 0600, 1)) == SEM_FAILED)
		return (ERROR);
	if ((params->sem_exit_code = sem_open(\
			SEMNAME_EXIT, O_CREAT, 0600, 1)) == SEM_FAILED)
		return (ERROR);
	return (SUCCESS);
}
