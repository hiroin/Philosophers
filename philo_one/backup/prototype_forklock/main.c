/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/07 14:01:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

bool	death_confirmation(struct timeval now, t_philosopher *phs)
{
	if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_die)
	{
		if (!(phs->params->someone_died))
		{
			write_message(convert_to_milliseconds(now), DIED, phs);
		}
		phs->params->someone_died = true;
		return (true);
	}	
	return (false);
}

void	*work(void *args)
{
	t_philosopher	*phs;
	struct timeval	now;
	pthread_t		died_check_thread;

	phs = args;
	gettimeofday(&now, NULL);
	phs->start_time_to_eat = now;
	write_message(convert_to_milliseconds(phs->start_time_to_eat), START, phs);
	while (1)
	{
		//だれかの死亡確認
		if (phs->params->someone_died == true)
			return (NULL);
		//自分の死亡確認
		if (death_confirmation(now, phs))
			return (NULL);
		// フォークをとる
		while (1)
		{
			pthread_mutex_lock(&phs->params->m_fork);
			if (phs->params->forks[phs->num] == false)
			{
				phs->params->forks[phs->num] = true;
				write_message(convert_to_milliseconds(now), FORK, phs);
				pthread_mutex_unlock(&phs->params->m_fork);
				break;
			}
			pthread_mutex_unlock(&phs->params->m_fork);
			if (phs->params->someone_died == true)
				return (NULL);
			gettimeofday(&now, NULL);
			if (death_confirmation(now, phs))
				return (NULL);
		}
		while (1)
		{
			pthread_mutex_lock(&phs->params->m_fork);
			if (phs->params->forks[(phs->num + 1) % phs->params->number_of_phs] == false)
			{
				phs->params->forks[(phs->num + 1) % phs->params->number_of_phs] = true;
				write_message(convert_to_milliseconds(now), FORK, phs);
				pthread_mutex_unlock(&phs->params->m_fork);
				break;
			}
			pthread_mutex_unlock(&phs->params->m_fork);
			if (phs->params->someone_died == true)
				return (NULL);
			gettimeofday(&now, NULL);
			if (death_confirmation(now, phs))
				return (NULL);
		}
		// 食事
		phs->start_time_to_eat = now;
		write_message(convert_to_milliseconds(now), EAT, phs);
		phs->num_of_eat++;
		usleep(phs->params->time_to_eat * 1000);
		// フォークをはなす
		pthread_mutex_lock(&phs->params->m_fork);
		phs->params->forks[phs->num] = false;
		phs->params->forks[(phs->num + 1) % phs->params->number_of_phs] = false;
		pthread_mutex_unlock(&phs->params->m_fork);
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
			break;
		// 睡眠をとる
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), SLEEP, phs);
		while (1)
		{
			usleep(1000);
			gettimeofday(&now, NULL);
			if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat + phs->params->time_to_sleep)
				break;
		}
		// 考える
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), THINK, phs);
	}
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), END, phs);
	return (NULL);
}

int			start_simulation(t_params params)
{
	t_philosopher	*phs;
	pthread_t		*threads;
	int				i;

	if ((phs = malloc(sizeof(t_philosopher) * params.number_of_phs)) == NULL)
		return (ERROR);
	if ((threads = malloc(sizeof(pthread_t) * params.number_of_phs)) == NULL)
	{
		free(phs);
		return (ERROR);
	}
	if ((params.forks = malloc(sizeof(bool) * params.number_of_phs)) == NULL)
	{
		free(phs);
		free(threads);
		return (ERROR);
	}
	pthread_mutex_init(&params.m_fork, NULL);
	i = 0;
	while (i < params.number_of_phs)
	{
		params.forks[i] = false;
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		if (init_t_philosopher(&phs[i], i, params))
		{
			free(phs);
			free(threads);
			return (ERROR);
		}
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		if (pthread_create(threads + i, NULL, work, phs + i))
			return (ERROR);
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		if (pthread_join(threads[i++], NULL))
			return (ERROR);
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		if (pthread_mutex_destroy(&params.m_fork))
			return (ERROR);
		i++;
	}
	free(params.forks);
	free(phs);
	free(threads);
	return (SUCCESS);
}

/*
** テスト関数
** gcc -g -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_one
** ./philo_one 4 31 10 20 10
** ./philo_one 4 3000 10 20 10
** valgrind --leak-check=full ./philo_one 4 3100 10 20 1
** valgrind --leak-check=full ./philo_one 4 3100 10 20 10
*/

int				main(int argc, char **argv)
{
	t_params		params;

	if (!(argc >= 5 && argc <= 6))
		return (ERROR);
	if (validate_argv(argc, argv))
		return (ERROR);
	if (set_t_params(&params, argc, argv))
		return (ERROR);
	pthread_mutex_init(&params.m_died, NULL);
	if (start_simulation(params))
	{
		pthread_mutex_destroy(&params.m_died);
		return (ERROR);
	}
	pthread_mutex_destroy(&params.m_died);
	return (SUCCESS);
}
