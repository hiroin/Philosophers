/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/13 07:46:31 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

bool	death_confirmation(struct timeval now, t_philosopher *phs)
{
	int		flag_bool;

	pthread_mutex_lock(&phs->params->m_params); //追加
	flag_bool = diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_die;
	pthread_mutex_unlock(&phs->params->m_params); //追加
	if (flag_bool)
	{
		pthread_mutex_lock(&phs->params->m_died); //追加
		flag_bool = phs->params->someone_died;
		pthread_mutex_unlock(&phs->params->m_died); //追加
		if (!flag_bool)
		{
			write_message(convert_to_milliseconds(now), DIED, phs);
		}
		//phs->params->someone_died = true;
		return (true);
	}
	return (false);
}

void	*died_check(void *args)
{
	t_philosopher	*phs;
	struct timeval	now;

	phs = args;
	usleep(10000);
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), MONITORSTART, phs);
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
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
		{
			pthread_mutex_unlock(&phs->params->m_params);
			return (NULL);
		}
		pthread_mutex_unlock(&phs->params->m_params);
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
	pthread_mutex_lock(&phs->params->m_params); //追加
	//pthread_create(&died_check_thread, NULL, died_check, phs);
	pthread_mutex_unlock(&phs->params->m_params); //追加
	while (1)
	{
		//だれかの死亡確認
		pthread_mutex_lock(&phs->params->m_died);
		if (phs->params->someone_died == true)
		{
			pthread_mutex_unlock(&phs->params->m_died);
			//pthread_join(died_check_thread, NULL);
			return (NULL);
		}
		pthread_mutex_unlock(&phs->params->m_died);
		// フォークをとる
		//pthread_mutex_lock(phs->params->m_fork[phs->num]);
		pthread_mutex_lock(&phs->params->m_fork[phs->num]);
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), FORK, phs);
		pthread_mutex_lock(&phs->params->m_fork[(phs->num + 1) % phs->params->number_of_phs]);
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), FORK, phs);
		// 食事
		pthread_mutex_lock(&phs->params->m_params);
		phs->start_time_to_eat = now;
		pthread_mutex_unlock(&phs->params->m_params);
		write_message(convert_to_milliseconds(now), EAT, phs);
		pthread_mutex_lock(&phs->params->m_params);
		phs->num_of_eat++;
		pthread_mutex_unlock(&phs->params->m_params);
		usleep(phs->params->time_to_eat * 1000);
		// フォークをはなす
		pthread_mutex_unlock(&phs->params->m_fork[phs->num]);
		pthread_mutex_unlock(&phs->params->m_fork[(phs->num + 1) % phs->params->number_of_phs]);
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
			break ;
		// 睡眠をとる
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), SLEEP, phs);
		while (1)
		{
			usleep(1000);
			gettimeofday(&now, NULL);
			if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat + phs->params->time_to_sleep)
				break ;
		}
		// 考える
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), THINK, phs);
	}
	gettimeofday(&now, NULL);
	write_message(convert_to_milliseconds(now), END, phs);
	//pthread_join(died_check_thread, NULL);
	return (NULL);
}

int			start_simulation(t_params *params)
{
	t_philosopher	*phs;
	pthread_t		*threads;
	pthread_t		*died_check_thread;
	int				i;

	if ((phs = malloc(sizeof(t_philosopher) * params->number_of_phs)) == NULL)
		return (ERROR);
	if ((threads = malloc(sizeof(pthread_t) * params->number_of_phs)) == NULL)
	{
		free(phs);
		return (ERROR);
	}
	died_check_thread = malloc(sizeof(pthread_t) * params->number_of_phs);
	if ((params->m_fork = malloc(sizeof(pthread_mutex_t) * params->number_of_phs)) == NULL)
	{
		free(phs);
		free(threads);
		return (ERROR);
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		pthread_mutex_init(&params->m_fork[i], NULL);
		i++;
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		if (init_t_philosopher(&phs[i], i, params))
		{
			free(phs);
			free(threads);
			free(params->m_fork);
			return (ERROR);
		}
		i++;
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		pthread_create(threads + i, NULL, work, phs + i);
		pthread_create(died_check_thread + i, NULL, died_check, phs + i);
		i++;
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		pthread_join(threads[i], NULL);
		pthread_join(died_check_thread[i], NULL);
		i++;
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		pthread_mutex_destroy(&params->m_fork[i]);
		i++;
	}
	free(params->m_fork);
	free(phs);
	free(threads);
	free(died_check_thread);
	return (SUCCESS);
}

/*
** テスト関数
** gcc -g -fsanitize=undefined,address -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_one
** gcc -g -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_one
** ./philo_one 4 31 10 20
** ./philo_one 4 31 10 20 10
** ./philo_one 4 3000 10 20 10
** valgrind --leak-check=full ./philo_one 4 3100 10 20 1
** valgrind --leak-check=full ./philo_one 4 3100 10 20 10
** valgrind --tool=helgrind ./philo_one 4 3100 20 10 1
** valgrind --tool=helgrind ./philo_one 4 3100 20 10 10
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
	pthread_mutex_init(&params.m_params, NULL);
	if (start_simulation(&params))
	{
		pthread_mutex_destroy(&params.m_died);
		return (ERROR);
	}
	pthread_mutex_destroy(&params.m_died);
	pthread_mutex_destroy(&params.m_params);
	return (SUCCESS);
}