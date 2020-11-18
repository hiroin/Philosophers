/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 07:10:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static bool	work_died_check(t_philosopher *phs)
{
	sem_wait(phs->params->sem_died);
	if (phs->params->someone_died == true)
	{
		sem_post(phs->params->sem_died);
		return (true);
	}
	sem_post(phs->params->sem_died);
	return (false);
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
			break ;
	}
}

void		*work(void *args)
{
	t_philosopher	*phs;

	phs = args;
	work_start(phs);
	while (1)
	{
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
	sem_wait(phs->params->sem_exit_code);
	phs->exit_code = EXIT_END;
	sem_post(phs->params->sem_exit_code);
	return (NULL);
}
