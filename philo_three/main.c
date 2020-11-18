/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 07:47:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static bool		allocation_memory_and_open_sem(t_params *params)
{
	if ((params->child_pids =
			malloc(sizeof(pid_t *) * params->number_of_phs)) == NULL)
		return (ERROR);
	if ((params->sem_forks = sem_open(\
			SEMNAME_FORK, O_CREAT, 0600, params->number_of_phs)) == SEM_FAILED)
		return (ERROR);
	if ((params->sem_died = sem_open(\
			SEMNAME_DIED, O_CREAT, 0600, 1)) == SEM_FAILED)
		return (ERROR);
	if ((params->sem_params = sem_open(\
			SEMNAME_PARAMS, O_CREAT, 0600, 1)) == SEM_FAILED)
		return (ERROR);
	if ((params->sem_exit_code = sem_open(\
			SEMNAME_EXIT, O_CREAT, 0600, 1)) == SEM_FAILED)
		return (ERROR);
	return (SUCCESS);
}

static int		start_simulation(t_params params)
{
	t_philosopher	phs;
	pthread_t		phs_threads;
	pthread_t		died_check_thread;
	int				i;
	int				sval;

	if (allocation_memory_and_open_sem(&params))
		return (ERROR);
	i = 0;
	while (i < params.number_of_phs)
	{
		init_t_philosopher(&phs, i, &params);
		if ((params.pid = fork()) == -1)
			return (ERROR);
		if (params.pid == 0)
		{
			pthread_create(&phs_threads, NULL, work, &phs);
			pthread_detach(phs_threads);
			usleep(100);
			pthread_create(&died_check_thread, NULL, died_check, &phs);
			pthread_detach(died_check_thread);
			while (1)
			{
				sem_wait(params.sem_exit_code);
				sem_getvalue(params.sem_exit_code, &sval);
				setbuf(stdout, NULL);
				printf("i = %d\n", i + 1);
				printf("sem = %d\n", sval);
				printf("phs.exit_code = %d\n", phs.exit_code);
				if (phs.exit_code != -1)
				{
					sem_post(params.sem_exit_code);
					break ;
				}
				sem_post(params.sem_exit_code);
				usleep(50);
			}
			if (phs.exit_code == EXIT_DIED)
				exit(EXIT_DIED);
			else
				exit(EXIT_END);
		}
		params.child_pids[i] = params.pid;
		i++;
	}
	i = 0;
	while (i < params.number_of_phs)
	{
		waitpid(-1, &params.status, 0);
		printf("OK\n");
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
