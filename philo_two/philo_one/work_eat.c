/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_eat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 01:28:28 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	work_eat_sleep(t_philosopher *phs)
{
	struct timeval	now;

	while (1)
	{
		usleep(50);
		pthread_mutex_lock(&phs->params->m_params);
		gettimeofday(&now, NULL);
		if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat)
		{
			pthread_mutex_unlock(&phs->params->m_params);
			break ;
		}
		pthread_mutex_unlock(&phs->params->m_params);
	}
}

void		work_eat(t_philosopher *phs)
{
	struct timeval	now;

	pthread_mutex_lock(&phs->params->m_fork[phs->num]);
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), FORK, phs);
	pthread_mutex_lock(&phs->params->m_fork[(phs->num + 1) \
						% phs->params->number_of_phs]);
	pthread_mutex_lock(&phs->params->m_params);
	gettimeofday(&now, NULL);
	phs->start_time_to_eat = now;
	write_message(convert_to_milliseconds(now), FORK, phs);
	write_message(convert_to_milliseconds(now), EAT, phs);
	phs->num_of_eat++;
	pthread_mutex_unlock(&phs->params->m_params);
	work_eat_sleep(phs);
	gettimeofday(&now, NULL);
	pthread_mutex_unlock(&phs->params->m_fork[phs->num]);
	pthread_mutex_unlock(&phs->params->m_fork[(phs->num + 1) \
						% phs->params->number_of_phs]);
}
