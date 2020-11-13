/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/04 01:18:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	write_message(long time, int phs_num, char *message)
{
	char	*s;
	char	*num;
	char	*tmp;

	num = ft_ltoa(time);
	s = ft_strjoin(num, " ");
	free(num);
	num = ft_ltoa(phs_num + 1);
	tmp = s;
	s = ft_strjoin(s, num);
	free(num);
	free(tmp);
	tmp = s;
	s = ft_strjoin(s, message);
	free(tmp);
	ft_putstr(s);
	free(s);
}
