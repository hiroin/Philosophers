/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_confirmation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 07:17:22 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

bool	death_confirmation(struct timeval now, t_philosopher *phs)
{
	int		flag_bool;

	sem_wait(phs->params->sem_params);
	flag_bool =
		diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_die;
	if (flag_bool)
	{
		sem_wait(phs->params->sem_died);
		flag_bool = phs->params->someone_died;
		sem_post(phs->params->sem_died);
		if (!flag_bool)
		{
			write_message(convert_to_milliseconds(now), DIED, phs);
		}
		sem_post(phs->params->sem_params);
		return (true);
	}
	sem_post(phs->params->sem_params);
	return (false);
}
