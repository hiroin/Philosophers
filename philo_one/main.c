/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 01:27:18 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static bool		allocation_memory(
					t_params *params,
					t_philosopher **phs,
					pthread_t **threads,
					pthread_t **died_check_thread)
{
	if ((*phs = malloc(sizeof(t_philosopher) * params->number_of_phs)) == NULL)
		return (ERROR);
	if ((*threads = malloc(sizeof(pthread_t) * params->number_of_phs)) == NULL)
		return (ERROR);
	if ((*died_check_thread =
			malloc(sizeof(pthread_t) * params->number_of_phs)) == NULL)
		return (ERROR);
	if ((params->m_fork =
			malloc(sizeof(pthread_mutex_t) * params->number_of_phs)) == NULL)
		return (ERROR);
	return (SUCCESS);
}

static bool		threads_join(
	t_params *params,
	pthread_t *threads,
	pthread_t *died_check_thread)
{
	int		i;

	i = 0;
	while (i < params->number_of_phs)
	{
		if (pthread_join(threads[i], NULL))
			return (ERROR);
		if (pthread_join(died_check_thread[i], NULL))
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

static bool		threads_create(
	t_params *params,
	t_philosopher *phs,
	pthread_t *threads,
	pthread_t *died_check_thread)
{
	int		i;

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
	return (SUCCESS);
}

static int		start_simulation(t_params *params)
{
	t_philosopher	*phs;
	pthread_t		*threads;
	pthread_t		*died_check_thread;
	int				i;

	if (allocation_memory(params, &phs, &threads, &died_check_thread))
		return (ERROR);
	i = 0;
	while (i < params->number_of_phs)
	{
		init_t_philosopher(&phs[i], i, params);
		if (pthread_mutex_init(&params->m_fork[i++], NULL))
			return (ERROR);
	}
	if (threads_create(params, phs, threads, died_check_thread))
		return (ERROR);
	if (threads_join(params, threads, died_check_thread))
		return (ERROR);
	i = 0;
	while (i < params->number_of_phs)
		pthread_mutex_destroy(&params->m_fork[i++]);
	free_memory(params, phs, threads, died_check_thread);
	return (SUCCESS);
}

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
		pthread_mutex_destroy(&params.m_params);
		exit(ERROR);
	}
	pthread_mutex_destroy(&params.m_died);
	pthread_mutex_destroy(&params.m_params);
	return (SUCCESS);
}
