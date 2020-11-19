/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_philosopher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 07:19:12 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 05:58:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int				init_t_philosopher(
	t_philosopher *philosopher,
	unsigned num,
	t_params *params)
{
	philosopher->num = num;
	philosopher->num_of_eat = 0;
	philosopher->exit_code = -1;
	philosopher->params = params;
	return (SUCCESS);
}
