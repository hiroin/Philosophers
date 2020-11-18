/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 08:19:56 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int		create_philosopher_threads(t_philosopher *phs)
{
	pthread_t		phs_threads;
	pthread_t		died_check_thread;

	if (pthread_create(&phs_threads, NULL, work, phs))
		return (ERROR);
	if (pthread_detach(phs_threads))
		return (ERROR);
	usleep(100);
	if (pthread_create(&died_check_thread, NULL, died_check, phs))
		return (ERROR);
	if (pthread_detach(died_check_thread))
		return (ERROR);
	return (SUCCESS);
}

void			check_exit_process(t_params *params, t_philosopher *phs)
{
	while (1)
	{
		sem_wait(params->sem_exit_code);
		if (phs->exit_code != -1)
		{
			sem_post(params->sem_exit_code);
			break ;
		}
		sem_post(params->sem_exit_code);
		usleep(50);
	}
}

static int		fork_philosophers(t_params *params)
{
	t_philosopher	phs;
	int				i;

	i = 0;
	while (i < params->number_of_phs)
	{
		init_t_philosopher(&phs, i, params);
		if ((params->pid = fork()) == -1)
			return (ERROR);
		if (params->pid == 0)
		{
			if (create_philosopher_threads(&phs))
				return (ERROR);
			check_exit_process(params, &phs);
			if (phs.exit_code == EXIT_DIED)
				exit(EXIT_DIED);
			else
				exit(EXIT_END);
		}
		params->child_pids[i] = params->pid;
		i++;
	}
	return (SUCCESS);
}

static int		start_simulation(t_params params)
{
	int				i;

	if (allocation_memory_and_open_sem(&params))
		return (ERROR);
	if (fork_philosophers(&params))
		return (ERROR);
	i = 0;
	while (i < params.number_of_phs)
	{
		waitpid(-1, &params.status, 0);
		if (WEXITSTATUS(params.status) == EXIT_DIED)
			break ;
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		kill(params.child_pids[i], SIGTERM);
		i++;
	}
	free_memory_and_close_sem(&params);
	return (SUCCESS);
}

int				main(int argc, char **argv)
{
	t_params		params;

	sem_unlink(SEMNAME_FORK);
	sem_unlink(SEMNAME_DIED);
	sem_unlink(SEMNAME_PARAMS);
	sem_unlink(SEMNAME_EXIT);
	if (!(argc >= 5 && argc <= 6))
		return (ERROR);
	if (validate_argv(argc, argv))
		return (ERROR);
	if (set_t_params(&params, argc, argv))
		return (ERROR);
	if (start_simulation(params))
	{
		exit(ERROR);
	}
	sem_unlink(SEMNAME_FORK);
	sem_unlink(SEMNAME_DIED);
	sem_unlink(SEMNAME_PARAMS);
	sem_unlink(SEMNAME_EXIT);
	return (SUCCESS);
}
