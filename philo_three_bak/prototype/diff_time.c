/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diff_time.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:35:09 by user42            #+#    #+#             */
/*   Updated: 2020/11/02 03:08:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/*
** 時刻の差分をミリ秒で返す関数
*/

long			diff_time(struct timeval end, struct timeval start)
{
	long	sec;
	long	microsec;
	long	diff_time;

	sec = end.tv_sec - start.tv_sec;
	microsec = end.tv_usec - start.tv_usec;
	diff_time = sec * 1000 + microsec / 1000;
	return (diff_time);
}
/*
** int				main(void)
** {
** 	struct timeval start_time;
** 	struct timeval end_time;
** 	gettimeofday(&start_time, NULL);
** 	usleep(1000);
** 	gettimeofday(&end_time, NULL);
** 	printf("1900/01/01からの秒 : %10ld\n", start_time.tv_sec);
** 	printf("        マイクロ秒 : %10ld\n", start_time.tv_usec);
** 	printf("1900/01/01からの秒 : %10ld\n", end_time.tv_sec);
** 	printf("        マイクロ秒 : %10ld\n", end_time.tv_usec);
** 	printf("diff_time = %ld\n", diff_time(end_time, start_time));
** 	return (0);
** }
*/
