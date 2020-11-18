/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   died_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 02:38:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		*died_check_loop(t_philosopher *phs)
{
	struct timeval	now;

	while (1)
	{
		gettimeofday(&now, NULL);
		if (death_confirmation(now, phs))
			return (NULL);
		sem_wait(phs->params->sem_died);
		if (phs->params->someone_died == true)
		{
			sem_post(phs->params->sem_died);
			return (NULL);
		}
		sem_post(phs->params->sem_died);
		sem_wait(phs->params->sem_params);
		if (phs->params->flag_set_max_number_of_eat == true
			&& phs->num_of_eat == phs->params->max_number_of_eat)
		{
			sem_post(phs->params->sem_params);
			return (NULL);
		}
		sem_post(phs->params->sem_params);
		usleep(50);
	}
}

void			*died_check(void *args)
{
	t_philosopher	*phs;

	phs = args;
	usleep(10000);
	return (died_check_loop(phs));
}
