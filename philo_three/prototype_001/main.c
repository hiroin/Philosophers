/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/13 02:53:17 by user42           ###   ########.fr       */
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
			phs->params->someone_died = true;
		}
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
	while (1)
	{
		gettimeofday(&now, NULL);
		if (death_confirmation(now, phs))
		{
			phs->exit_code = EXIT_DIED;
			return (NULL);
		}
		// if (phs->params->someone_died == true)
		// {
		// 	return (NULL);
		// }
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
		{
			return (NULL);
		}
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
	pthread_create(&died_check_thread, NULL, died_check, phs);
	pthread_detach(died_check_thread);
	while (1)
	{
		//自分の死亡確認
		if (phs->params->someone_died == true)
			return (NULL);
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
		phs->start_time_to_eat = now;
		write_message(convert_to_milliseconds(now), EAT, phs);
		setbuf(stdout, NULL);
		phs->num_of_eat++;
		usleep(phs->params->time_to_eat * 1000);
		// フォークをはなす
		sem_post(phs->params->sem_forks);
		sem_post(phs->params->sem_forks);
		if (phs->params->flag_set_max_number_of_eat == true && phs->num_of_eat == phs->params->max_number_of_eat)
		{
			break ;
		}
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
	phs->exit_code = EXIT_END;
	//pthread_join(died_check_thread, NULL);
	return (NULL);
}

int			start_simulation(t_params params)
{
	t_philosopher	phs;
	int				i;
	pid_t			*child_pids;
	pid_t			pid;
	pthread_t		phs_threads;
	int				status;

	child_pids = malloc(sizeof(pid_t *) * params.number_of_phs);
	if (!child_pids)
		return (ERROR);
	params.sem_forks = sem_open(SEMNAME_FORK, O_CREAT , 0600, params.number_of_phs);
	if (params.sem_forks == SEM_FAILED)
		return (ERROR);
	params.sem_died = sem_open(SEMNAME_DIED, O_CREAT , 0600, 1);
	if (params.sem_died == SEM_FAILED)
	{
		sem_close(params.sem_forks);
		sem_unlink(SEMNAME_FORK);
		return (ERROR);
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		init_t_philosopher(&phs, i, &params);
		pid = fork();
		if (pid == -1)
			return (ERROR);
		if (pid == 0)
		{
			pthread_create(&phs_threads, NULL, work, &phs);
			//usleep(1000);
			//pthread_create(&died_check_threads, NULL, died_check, &phs);
			pthread_detach(phs_threads);
			//pthread_detach(died_check_threads);
			while (1)
			{
				if (phs.exit_code != -1)
					break;
			}
			if (phs.exit_code == EXIT_DIED)
				exit(EXIT_DIED);
			else
				exit(EXIT_END);
		}
		child_pids[i] = pid;
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		waitpid(-1 , &status, 0);
		//printf("status = %d\n", WEXITSTATUS(status));
		if (WEXITSTATUS(status) == EXIT_DIED)
			break;
			i++;
	}	
	i = 0;
	while (i < params.number_of_phs)
	{
		kill(child_pids[i], SIGTERM);
		//printf("Child Process : %d kill\n", i);
		i++;
	}	
	sem_close(params.sem_forks);
	sem_close(params.sem_died);
	sem_unlink(SEMNAME_FORK);
	sem_unlink(SEMNAME_DIED);
	free(child_pids);
	return (SUCCESS);
}

/*
** テスト関数
** gcc -Wall -Werror -Wextra -g -fsanitize=undefined,address -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_three
** gcc -Wall -Werror -Wextra -g -pthread main.c diff_time.c ft_strlen.c ft_putstr.c convert_to_milliseconds.c validate_argv.c init_t_philosopher.c ft_atoi.c set_t_params.c ft_ltoa.c ft_strjoin.c write_message.c -o philo_three
** ./philo_three 4 31 10 20 10
** ./philo_three 4 3000 10 20 10
** valgrind --leak-check=full ./philo_three 4 3100 10 20 1
** valgrind --leak-check=full ./philo_three 4 3100 10 20 10
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
	if (start_simulation(params))
	{
		return (ERROR);
	}
	return (SUCCESS);
}
