/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/05 01:23:34 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

pthread_mutex_t *g_m;

bool	death_confirmation(struct timeval now, t_philosopher *phs)
{
	if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_die)
	{
		if (!(phs->params->someone_died))
		{
			//write_message(convert_to_milliseconds(now), phs->num, " died\n", 1);
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
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), MONITORSTART, phs);
	if (death_confirmation(now, phs))
	{
		write_message(convert_to_milliseconds(now), MONITORSTART, phs);
		return (NULL);	
	}
	return (NULL);
}

void	*work(void *args)
{
	t_philosopher	*phs;
	struct timeval	now;
	int				i;
	pthread_t		died_check_thread;

	phs = args;
	gettimeofday(&now, NULL);
	phs->start_time_to_eat = now;
	//printf("%ld %d start\n", convert_to_milliseconds(phs->start_time_to_eat), phs->num + 1);
	//write_message(convert_to_milliseconds(phs->start_time_to_eat), phs->num, " start\n", !(phs->params->someone_died));
	write_message(convert_to_milliseconds(phs->start_time_to_eat), START, phs);
	pthread_create(&died_check_thread, NULL, died_check, phs);
	pthread_detach(died_check_thread);
	while (1)
	{
		//だれかの死亡確認
		if (phs->params->someone_died == true)
			return (NULL);
		// フォークをとる
		while(pthread_mutex_trylock(&g_m[phs->num]))
		{
			gettimeofday(&now, NULL);
			//printf("%ld %d has wait a fork\n", convert_to_milliseconds(now), phs->num + 1);
			if (death_confirmation(now, phs))
				return (NULL);
		}
		gettimeofday(&now, NULL);
		//printf("%ld %d has taken a fork\n", convert_to_milliseconds(now), phs->num + 1);
		//write_message(convert_to_milliseconds(now), phs->num, " has taken a fork\n", !(phs->params->someone_died));
		write_message(convert_to_milliseconds(now), FORK, phs);
		while(pthread_mutex_trylock(&g_m[(phs->num + 1) % phs->params->number_of_phs]))
		{
			gettimeofday(&now, NULL);
			if (death_confirmation(now, phs))
				return (NULL);
		}
		gettimeofday(&now, NULL);
		//printf("%ld %d has taken a fork\n", convert_to_milliseconds(now), phs->num + 1);
		//write_message(convert_to_milliseconds(now), phs->num, " has taken a fork\n", !(phs->params->someone_died));
		write_message(convert_to_milliseconds(now), FORK, phs);
		// 食事
		phs->start_time_to_eat = now;
		//printf("%ld %d is eating\n", convert_to_milliseconds(now), phs->num + 1);
		//write_message(convert_to_milliseconds(now), phs->num, " is eating\n", !(phs->params->someone_died));
		write_message(convert_to_milliseconds(now), EAT, phs);
		phs->num_of_eat++;
		usleep(phs->params->time_to_eat * 1000);
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
		{
			pthread_mutex_unlock(&g_m[phs->num]);
			pthread_mutex_unlock(&g_m[(phs->num + 1) % phs->params->number_of_phs]);
			break;
		}
		// フォークをはなす
		pthread_mutex_unlock(&g_m[phs->num]);
		pthread_mutex_unlock(&g_m[(phs->num + 1) % phs->params->number_of_phs]);
		// 睡眠をとる
		gettimeofday(&now, NULL);
		//printf("%ld %d is sleeping\n", convert_to_milliseconds(now), phs->num + 1);
		//write_message(convert_to_milliseconds(now), phs->num, " is sleeping\n", !(phs->params->someone_died));
		write_message(convert_to_milliseconds(now), SLEEP, phs);
		while (1)
		{
			usleep(1000);
			gettimeofday(&now, NULL);
			if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat + phs->params->time_to_sleep)
				break;
		}
		//usleep(phs->params->time_to_sleep * 1000);
		// 考える
		gettimeofday(&now, NULL);
		//printf("%ld %d is thinking\n", convert_to_milliseconds(now), phs->num + 1);
		//write_message(convert_to_milliseconds(now), phs->num, " is thinking\n", !(phs->params->someone_died));
		write_message(convert_to_milliseconds(now), THINK, phs);
	}
	gettimeofday(&now, NULL);
	//printf("%ld %d end\n", convert_to_milliseconds(now), phs->num + 1);
	//write_message(convert_to_milliseconds(now), phs->num, " end\n", !(phs->params->someone_died));
	write_message(convert_to_milliseconds(now), END, phs);
	return (NULL);
}

int			start_simulation(t_params params)
{
	t_philosopher	*phs;
	pthread_t		*threads;
	int				i;

	phs = malloc(sizeof(t_philosopher) * params.number_of_phs);
	threads = malloc(sizeof(pthread_t) * params.number_of_phs * 2);
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
	g_m = malloc(sizeof(pthread_mutex_t) * params.number_of_phs);
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_mutex_init(&g_m[i], NULL);
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_create(threads + i, NULL, work, phs + i);
		//pthread_create(threads + params.number_of_phs + i, NULL, died_check, phs + i);
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_join(threads[i++], NULL);
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		pthread_mutex_destroy(&g_m[i]);
		i++;
	}
	free(phs);
	free(threads);
	return (SUCCESS);
}

/*
** テスト関数
** gcc -g -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c
** ./a.out 2 1000 10 20 10
** valgrind --leak-check=full ./a.out 2 1000 10 20 10
*/

int				main(int argc, char **argv)
{
	t_params		params;
	struct timeval	start_time;
	struct timeval	end_time;

	if (!(argc >= 5 && argc <= 6))
		return (ERROR);
	if (validate_argv(argc, argv))
		return (ERROR);
	if (set_t_params(&params, argc, argv))
		return (ERROR);
	gettimeofday(&start_time, NULL);
	pthread_mutex_init(&params.m_died, NULL);
	if (start_simulation(params))
	{
		pthread_mutex_destroy(&params.m_died);
		return (ERROR);
	}
	pthread_mutex_destroy(&params.m_died);
	gettimeofday(&end_time, NULL);
	printf("スタートから%ldミリ秒経過\n", diff_time(end_time, start_time));
	return (SUCCESS);
}
