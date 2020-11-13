/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 07:17:30 by user42            #+#    #+#             */
/*   Updated: 2020/11/03 13:12:29 by user42           ###   ########.fr       */
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

int				validate_argv(int argc, char **argv)
{
	int		tmp;

	tmp = ft_atoi(argv[1]);
	if (tmp <= 0)
		return (ERROR);
	tmp = ft_atoi(argv[2]);
	if (tmp <= 0)
		return (ERROR);
	tmp = ft_atoi(argv[3]);
	if (tmp <= 0)
		return (ERROR);
	tmp = ft_atoi(argv[4]);
	if (tmp <= 0)
		return (ERROR);
	if (argc == 6)
	{
		tmp = ft_atoi(argv[5]);
		if (tmp <= 0)
			return (ERROR);
	}
	return (SUCCESS);
}
