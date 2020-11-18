/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_t_params.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 13:18:46 by user42            #+#    #+#             */
/*   Updated: 2020/11/04 03:39:05 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
** Arguments
**  1 number_of_philosophers
**  2 time_to_die
**  3 time_to_eat
**  4 time_to_sleep
**  5 [number_of_times_each_philosopher_must_eat]
*/

int				set_t_params(t_params *params, int argc, char **argv)
{
	params->number_of_phs = ft_atoi(argv[1]);
	params->time_to_die = ft_atoi(argv[2]);
	params->time_to_eat = ft_atoi(argv[3]);
	params->time_to_sleep = ft_atoi(argv[4]);
	if (params->time_to_die <= params->time_to_eat + params->time_to_sleep)
	{
		return (ERROR);
	}
	if (argc == 6)
	{
		params->flag_set_max_number_of_eat = true;
		params->max_number_of_eat = ft_atoi(argv[5]);
	}
	params->someone_died = false;
	return (SUCCESS);
}
