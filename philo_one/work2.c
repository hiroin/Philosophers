/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 01:03:04 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	work_start(t_philosopher *phs)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	pthread_mutex_lock(&phs->params->m_params);
	phs->start_time_to_eat = now;
	pthread_mutex_unlock(&phs->params->m_params);
	write_message(convert_to_milliseconds(phs->start_time_to_eat), START, phs);
}

void	work_end(t_philosopher *phs)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), END, phs);
}
