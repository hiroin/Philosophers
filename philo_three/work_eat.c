/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_eat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 09:40:18 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	work_eat_sleep(t_philosopher *phs)
{
	struct timeval	now;

	while (1)
	{
		usleep(50);
		sem_wait(phs->params->sem_params);
		gettimeofday(&now, NULL);
		if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat)
		{
			sem_post(phs->params->sem_params);
			break ;
		}
		sem_post(phs->params->sem_params);
	}
}

void		work_eat(t_philosopher *phs)
{
	struct timeval	now;

	sem_wait(phs->params->sem_both_forks);
	sem_wait(phs->params->sem_forks);
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), FORK, phs);
	sem_wait(phs->params->sem_forks);
	sem_post(phs->params->sem_both_forks);
	sem_wait(phs->params->sem_params);
	gettimeofday(&now, NULL);
	phs->start_time_to_eat = now;
	write_message(convert_to_milliseconds(now), FORK, phs);
	write_message(convert_to_milliseconds(now), EAT, phs);
	phs->num_of_eat++;
	sem_post(phs->params->sem_params);
	work_eat_sleep(phs);
	gettimeofday(&now, NULL);
	sem_post(phs->params->sem_forks);
	sem_post(phs->params->sem_forks);
}
