/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_philosopher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 07:19:12 by user42            #+#    #+#             */
/*   Updated: 2020/11/08 02:42:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int				init_t_philosopher(
	t_philosopher *philosopher,
	unsigned num)
{
	philosopher->num = num;
	philosopher->num_of_eat = 0;
	return (SUCCESS);
}
