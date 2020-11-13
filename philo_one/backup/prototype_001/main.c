/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/02 07:29:49 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int				validate_argv(char **params)
{
	return (SUCCESS);
}

int				set_t_params(t_params *params, char **argv)
{
	params->number_of_phs = 2;
	params->time_to_die = 1000;
	params->time_to_eat = 10;
	params->time_to_sleep = 20;
	params->flag_set_max_number_of_eat = true;
	params->max_number_of_eat = 10;
	return (SUCCESS);
}

int				init_t_philosopher(
	t_philosopher *philosopher,
	unsigned num,
	t_params params)
{
	philosopher->num = num;
	philosopher->right_fork = false;
	philosopher->left_fork = false;
	philosopher->flag_getting_a_fork = false;
	philosopher->flag_eating = false;
	philosopher->flag_sleeping = false;
	philosopher->flag_set_max_num_of_eat = true;
	philosopher->num_of_eat = 0;
	philosopher->max_num_of_eat = params.max_number_of_eat;
	return (SUCCESS);
}

void	*work(void *args)
{
	t_philosopher	*phs;
	struct timeval	now;
	int				i;

	phs = args;
	gettimeofday(&now, NULL);
	phs->start_time_to_eat = now;
	printf("%ld %d start\n", convert_to_milliseconds(phs->start_time_to_eat), phs->num);

	while (phs->num_of_eat < phs->max_num_of_eat)
	{
		gettimeofday(&now, NULL);
		printf("%ld %d is eating\n", convert_to_milliseconds(now), phs->num);
		phs->num_of_eat++;
	}
	gettimeofday(&now, NULL);
	printf("%ld %d end\n", convert_to_milliseconds(now), phs->num);
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
	free(phs);
	free(threads);
	return (SUCCESS);
}

/*
** テスト関数
** gcc -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c
*/

int				main(int argc, char **argv)
{
	t_params	params;
	struct timeval start_time;
	struct timeval end_time;

	if (argc >= 5 && argc <= 6)
		return (ERROR);
	if (validate_argv(argv))
		return (ERROR);
	if (set_t_params(&params, argv))
		return (ERROR);
	gettimeofday(&start_time, NULL);
	if (start_simulation(params))
		return (ERROR);
	gettimeofday(&end_time, NULL);
	printf("スタートから%ldミリ秒経過\n", diff_time(end_time, start_time));
	return (0);
}
