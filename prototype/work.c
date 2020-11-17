/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/17 02:51:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static bool	work_died_check(t_philosopher *phs)
{
	pthread_mutex_lock(&phs->params->m_died);
	if (phs->params->someone_died == true)
	{
		pthread_mutex_unlock(&phs->params->m_died);
		return (true);
	}
	pthread_mutex_unlock(&phs->params->m_died);
	return (false);
}

static void	work_eat(t_philosopher *phs)
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
	// gettimeofday(&now, NULL);
	// write_message(convert_to_milliseconds(now), MONITORDIED, phs);	
	// usleep(phs->params->time_to_eat * 500);
	// gettimeofday(&now, NULL);
	// write_message(convert_to_milliseconds(now), MONITORDIED, phs);	
	// usleep(phs->params->time_to_eat * 500);
	while (1)
	{
		usleep(50);
		gettimeofday(&now, NULL);
		if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat)
			break ;
	}
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), MONITORSTART, phs);	
	pthread_mutex_unlock(&phs->params->m_fork[phs->num]);
	pthread_mutex_unlock(&phs->params->m_fork[(phs->num + 1) \
						% phs->params->number_of_phs]);
}

static void	work_think(t_philosopher *phs)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), THINK, phs);
}

static void	work_sleep(t_philosopher *phs)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), SLEEP, phs);
	while (1)
	{
		usleep(50);
		gettimeofday(&now, NULL);
		if (diff_time(now, phs->start_time_to_eat) \
					>= phs->params->time_to_eat + phs->params->time_to_sleep)
		{
			printf("diff = %ld\n", diff_time(now, phs->start_time_to_eat));
			break ;
		}
	}
}

void		*work(void *args)
{
	t_philosopher	*phs;

	phs = args;
	work_start(phs);
	while (1)
	{
		// if (work_died_check(phs))
		// 	return (NULL);
		work_eat(phs);
		if (phs->params->flag_set_max_number_of_eat == true
			&& phs->num_of_eat == phs->params->max_number_of_eat)
			break ;
		if (work_died_check(phs))
			return (NULL);
		work_sleep(phs);
		work_think(phs);
	}
	work_end(phs);
	return (NULL);
}
