/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 02:53:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void		free_memory_and_close_sem(
				t_params *params,
				t_philosopher *phs,
				pthread_t *threads,
				pthread_t *died_check_thread)
{
	free(phs);
	free(threads);
	free(died_check_thread);
	sem_close(params->sem_forks);
	sem_close(params->sem_died);
	sem_close(params->sem_params);
}
