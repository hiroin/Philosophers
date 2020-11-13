/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/11 03:02:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static char	*get_message(int kind)
{
	if (kind == FORK)
		return (" has taken a fork\n");
	else if (kind == EAT)
		return (" is eating\n");
	else if (kind == SLEEP)
		return (" is sleeping\n");
	else if (kind == THINK)
		return (" is thinking\n");
	else if (kind == DIED)
		return (" died\n");
	else if (kind == START)
		return (" start\n");
	else if (kind == END)
		return (" end\n");
	else if (kind == MONITORSTART)
		return (" moniter start\n");
	else if (kind == MONITORDIED)
		return (" moniter died\n");
	return ("");
}

void		write_message(long time, int message_kind, t_philosopher *phs)
{
	char	*s;
	char	*num;
	char	*tmp;

	sem_wait(g_sem_died);
	if (g_params.someone_died == false)
	{
		num = ft_ltoa(time);
		s = ft_strjoin(num, " ");
		free(num);
		num = ft_ltoa(phs->num + 1);
		tmp = s;
		s = ft_strjoin(s, num);
		free(num);
		free(tmp);
		tmp = s;
		s = ft_strjoin(s, get_message(message_kind));
		free(tmp);
		ft_putstr(s);
		free(s);
	}
	if (message_kind == DIED)
		g_params.someone_died = true;
	sem_post(g_sem_died);
}
