/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:08:26 by user42            #+#    #+#             */
/*   Updated: 2020/11/02 07:23:35 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <sys/time.h>
# include <stdbool.h>
# include <pthread.h>

# define SUCCESS 0
# define ERROR 1

typedef struct	s_params
{
	int		number_of_phs;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	bool	flag_set_max_number_of_eat;
	int		max_number_of_eat;
}				t_params;

typedef struct	s_philosopher
{
	int				num;
	bool			right_fork;
	bool			left_fork;
	bool			flag_getting_a_fork;
	bool			flag_eating;
	bool			flag_sleeping;
	struct timeval	start_time_to_eat;
	bool			flag_set_max_num_of_eat;
	int				num_of_eat;
	int				max_num_of_eat;
}				t_philosopher;

long	convert_to_milliseconds(struct timeval time);
int		ft_strlen(char *s);
void	ft_putstr(char *s);
long	diff_time(struct timeval end, struct timeval start);

#endif
