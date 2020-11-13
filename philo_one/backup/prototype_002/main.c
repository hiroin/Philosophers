/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/04 01:20:06 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

pthread_mutex_t *g_m;

bool	death_confirmation(struct timeval now, t_philosopher *phs)
{
	if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_die)
	{
		write_message(convert_to_milliseconds(now), phs->num, " died\n");
		return (true);
	}	
	return (false);
}

void	*work(void *args)
{
	t_philosopher	*phs;
	struct timeval	now;
	int				i;

	phs = args;
	gettimeofday(&now, NULL);
	phs->start_time_to_eat = now;
	printf("%ld %d start\n", convert_to_milliseconds(phs->start_time_to_eat), phs->num + 1);
	while (1)
	{
		//死亡確認
		if (death_confirmation(now, phs))
			return (NULL);
		// フォークをとる
		while(pthread_mutex_lock(&g_m[phs->num]))
		{
			if (death_confirmation(now, phs))
				return (NULL);
		}
		gettimeofday(&now, NULL);
		printf("%ld %d has taken a fork\n", convert_to_milliseconds(now), phs->num + 1);
		while(pthread_mutex_lock(&g_m[(phs->num + 1) % phs->params->number_of_phs]))
		{
			if (death_confirmation(now, phs))
				return (NULL);
		}
		gettimeofday(&now, NULL);
		printf("%ld %d has taken a fork\n", convert_to_milliseconds(now), phs->num + 1);
		// 食事
		phs->start_time_to_eat = now;
		printf("%ld %d is eating\n", convert_to_milliseconds(now), phs->num + 1);
		phs->num_of_eat++;
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
		{
			pthread_mutex_unlock(&g_m[phs->num]);
			pthread_mutex_unlock(&g_m[(phs->num + 1) % phs->params->number_of_phs]);
			break;
		}
		usleep(phs->params->time_to_eat * 1000);
		// フォークをはなす
		pthread_mutex_unlock(&g_m[phs->num]);
		pthread_mutex_unlock(&g_m[(phs->num + 1) % phs->params->number_of_phs]);
		// 睡眠をとる
		gettimeofday(&now, NULL);
		printf("%ld %d is sleeping\n", convert_to_milliseconds(now), phs->num + 1);
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
		printf("%ld %d is thinking\n", convert_to_milliseconds(now), phs->num + 1);
	}
	gettimeofday(&now, NULL);
	printf("%ld %d end\n", convert_to_milliseconds(now), phs->num + 1);
	return (NULL);
}

int			start_simulation(t_params params)
{
	t_philosopher	*phs;
	pthread_t		*threads;
	int				i;

	phs = malloc(sizeof(t_philosopher) * params.number_of_phs);
	threads = malloc(sizeof(pthread_t) * params.number_of_phs);
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
	if (start_simulation(params))
		return (ERROR);
	gettimeofday(&end_time, NULL);
	printf("スタートから%ldミリ秒経過\n", diff_time(end_time, start_time));
	return (SUCCESS);
}
