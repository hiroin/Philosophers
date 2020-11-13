/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/06 05:45:05 by user42           ###   ########.fr       */
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

void	*died_check(void *args)
{
	t_philosopher	*phs;
	struct timeval	now;

	phs = args;
	while (phs->flag_start == false)
		usleep(1000);
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), MONITORSTART, phs);
	while (1)
	{
		gettimeofday(&now, NULL);
		if (death_confirmation(now, phs))
			return (NULL);
		if (phs->params->someone_died == true)
			return (NULL);
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
			return (NULL);
	}
	return (NULL);
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
	phs->flag_start = true;
	while (1)
	{
		// だれかの死亡確認
		if (phs->params->someone_died == true)
		{
			return (NULL);
		}
		// フォークをとる
		pthread_mutex_lock(&phs->params->m_fork[phs->num]);
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), FORK, phs);
		// printf("phs->num + 1 = %d\n", phs->num + 1);
		// printf("phs->params->number_of_phs = %d\n", phs->params->number_of_phs);
		pthread_mutex_lock(&phs->params->m_fork[(phs->num + 1) % phs->params->number_of_phs]);
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), FORK, phs);
		
		// 食事
		phs->start_time_to_eat = now;
		write_message(convert_to_milliseconds(now), EAT, phs);
		phs->num_of_eat++;
		usleep(phs->params->time_to_eat * 1000);
		// フォークをはなす
		pthread_mutex_unlock(&phs->params->m_fork[phs->num]);
		pthread_mutex_unlock(&phs->params->m_fork[(phs->num + 1) % phs->params->number_of_phs]);

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
	pthread_t		*died_threads;
	int				i;

	phs = malloc(sizeof(t_philosopher) * params.number_of_phs);
	threads = malloc(sizeof(pthread_t) * params.number_of_phs);
	died_threads = malloc(sizeof(pthread_t) * params.number_of_phs);
	params.m_fork = malloc(sizeof(pthread_mutex_t) * params.number_of_phs);
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_mutex_init(&params.m_fork[i], NULL);
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
		pthread_create(threads + i, NULL, work, phs + i);
		pthread_create(died_threads + i, NULL, died_check, phs + i);
		usleep(1000);
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_join(threads[i], NULL);
		pthread_join(died_threads[i], NULL);
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_mutex_destroy(&params.m_fork[i]);
		i++;
	}
	free(params.m_fork);
	free(phs);
	free(threads);
	free(died_threads);
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
