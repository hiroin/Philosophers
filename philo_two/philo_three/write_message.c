/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 07:37:26 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static char		*get_message(int kind)
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

static void		write_message_assembly(
						long time, int message_kind, t_philosopher *phs)
{
	char	*s;
	char	*num;
	char	*tmp;

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

void			write_message(long time, int message_kind, t_philosopher *phs)
{
	sem_wait(phs->params->sem_died);
	if (phs->params->someone_died == false)
		write_message_assembly(time, message_kind, phs);
	if (message_kind == DIED)
	{
		phs->params->someone_died = true;
		return ;
	}
	sem_post(phs->params->sem_died);
}
