/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diff_time.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 10:29:23 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

long			diff_time(struct timeval end, struct timeval start)
{
	long	sec;
	long	microsec;
	long	diff_time;

	sec = end.tv_sec - start.tv_sec;
	microsec = end.tv_usec / 1000 - start.tv_usec / 1000;
	diff_time = sec * 1000 + microsec;
	return (diff_time);
}
