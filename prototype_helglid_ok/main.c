/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 01:45:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

bool	death_confirmation(struct timeval now, t_philosopher *phs)
{
	int		flag_died;
	
	sem_wait(phs->params->sem_died); //追加
	flag_died = diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_die;
	sem_post(phs->params->sem_died); //追加
	if (flag_died)
	{
		if (!(phs->params->someone_died))
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
		sem_wait(phs->params->sem_died); //追加
		if (phs->params->someone_died == true)
		{
			sem_post(phs->params->sem_died); //追加
			return (NULL);
		}
		sem_post(phs->params->sem_died); //追加
		sem_wait(phs->params->sem_died); //追加
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
		{
			sem_post(phs->params->sem_died); //追加
			return (NULL);
		}
		sem_post(phs->params->sem_died); //追加
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
	//pthread_create(&died_check_thread, NULL, died_check, phs);
	while (1)
	{
		//だれかの死亡確認
		sem_wait(phs->params->sem_died);
		if (phs->params->someone_died == true)
		{
			sem_post(phs->params->sem_died);
			//pthread_join(died_check_thread, NULL);
			return (NULL);
		}
		sem_post(phs->params->sem_died);
		// フォークをとる
		//pthread_mutex_lock(&g_params.m_fork[phs->num]);
		sem_wait(phs->params->sem_forks);
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), FORK, phs);
		//pthread_mutex_lock(&g_params.m_fork[(phs->num + 1) % g_params.number_of_phs]);
		sem_wait(phs->params->sem_forks);
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), FORK, phs);
		// 食事
		sem_wait(phs->params->sem_died); //追加
		phs->start_time_to_eat = now;
		sem_post(phs->params->sem_died); //追加
		write_message(convert_to_milliseconds(now), EAT, phs);
		sem_wait(phs->params->sem_died); //追加
		phs->num_of_eat++;
		sem_post(phs->params->sem_died); //追加
		usleep(phs->params->time_to_eat * 1000);
		while (1)
		{
			usleep(50);
			sem_wait(phs->params->sem_died);
			gettimeofday(&now, NULL);
			if (diff_time(now, phs->start_time_to_eat) >= phs->params->time_to_eat)
			{
				sem_post(phs->params->sem_died);
				break ;
			}
			sem_post(phs->params->sem_died);
		}
		// フォークをはなす
		//pthread_mutex_unlock(&g_params.m_fork[phs->num]);
		sem_post(phs->params->sem_forks);
		//pthread_mutex_unlock(&g_params.m_fork[(phs->num + 1) % g_params.number_of_phs]);
		sem_post(phs->params->sem_forks);
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
			break ;
		// 睡眠をとる
		gettimeofday(&now, NULL);
		write_message(convert_to_milliseconds(now), SLEEP, phs);
		while (1)
		{
			usleep(50);
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
	params->sem_forks = sem_open(SEMNAME_FORK, O_CREAT , 0600, params->number_of_phs);
	if (params->sem_forks == SEM_FAILED)
	{
		free(phs);
		free(threads);
		return (ERROR);
	}
	params->sem_died = sem_open(SEMNAME_DIED, O_CREAT , 0600, 1);
	if (params->sem_died == SEM_FAILED)
	{
		free(phs);
		free(threads);
		sem_close(params->sem_forks);
		sem_unlink(SEMNAME_FORK);
		return (ERROR);
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		if (init_t_philosopher(&phs[i], i, params))
		{
			free(phs);
			free(threads);
			sem_close(phs->params->sem_forks);
			sem_close(phs->params->sem_died);
			sem_unlink(SEMNAME_FORK);
			sem_unlink(SEMNAME_DIED);
			return (ERROR);
		}
		i++;
	}
	i = 0;
	while (i < params->number_of_phs / 2 + params->number_of_phs % 2)
	{
		if (pthread_create(threads + (2 * i), NULL, work, phs + (2 * i)))
			return (ERROR);
		if (pthread_create(
			died_check_thread + (2 * i), NULL, died_check, phs + (2 * i)))
			return (ERROR);
		i++;
	}
	usleep(100);
	i = 0;
	while (i < params->number_of_phs / 2)
	{
		if (pthread_create(threads + (2 * i + 1),
							NULL, work, phs + (2 * i + 1)))
			return (ERROR);
		if (pthread_create(died_check_thread + (2 * i + 1),
							NULL, died_check, phs + (2 * i + 1)))
			return (ERROR);
		i++;
	}
	i = 0;
	while (i < params->number_of_phs)
	{
		pthread_join(threads[i], NULL);
		pthread_join(died_check_thread[i], NULL);
		i++;
	}
	sem_close(phs->params->sem_forks);
	sem_close(phs->params->sem_died);
	sem_unlink(SEMNAME_FORK);
	sem_unlink(SEMNAME_DIED);
	free(phs);
	free(threads);
	free(died_check_thread);
	return (SUCCESS);
}

/*
** テスト関数
** gcc -Wall -Werror -Wextra -g -fsanitize=undefined,address -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_two
** gcc -Wall -Werror -Wextra -g -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_two
** ./philo_one 4 31 10 20 10
** ./philo_one 4 3000 10 20 10
** valgrind --leak-check=full ./philo_one 4 3100 10 20 1
** valgrind --leak-check=full ./philo_one 4 3100 10 20 10
*/

int				main(int argc, char **argv)
{
	t_params		params;

	sem_unlink(SEMNAME_FORK);
	sem_unlink(SEMNAME_DIED);
	if (!(argc >= 5 && argc <= 6))
		return (ERROR);
	if (validate_argv(argc, argv))
		return (ERROR);
	if (set_t_params(&params, argc, argv))
		return (ERROR);
	if (start_simulation(&params))
	{
		return (ERROR);
	}
	return (SUCCESS);
}
