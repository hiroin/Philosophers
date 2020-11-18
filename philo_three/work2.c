/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 02:21:21 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	work_start(t_philosopher *phs)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	sem_wait(phs->params->sem_params);
	phs->start_time_to_eat = now;
	sem_post(phs->params->sem_params);
	write_message(convert_to_milliseconds(phs->start_time_to_eat), START, phs);
}

void	work_end(t_philosopher *phs)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), END, phs);
}
