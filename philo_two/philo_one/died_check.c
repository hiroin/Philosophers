/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   died_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 01:03:22 by user42           ###   ########.fr       */
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
		pthread_mutex_lock(&phs->params->m_died);
		if (phs->params->someone_died == true)
		{
			pthread_mutex_unlock(&phs->params->m_died);
			return (NULL);
		}
		pthread_mutex_unlock(&phs->params->m_died);
		pthread_mutex_lock(&phs->params->m_params);
		if (phs->params->flag_set_max_number_of_eat == true
			&& phs->num_of_eat == phs->params->max_number_of_eat)
		{
			pthread_mutex_unlock(&phs->params->m_params);
			return (NULL);
		}
		pthread_mutex_unlock(&phs->params->m_params);
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
