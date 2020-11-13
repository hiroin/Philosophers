/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:08:26 by user42            #+#    #+#             */
/*   Updated: 2020/11/04 01:19:10 by user42           ###   ########.fr       */
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
	int				num_of_eat;
	t_params		*params;
}				t_philosopher;

int				ft_atoi(const char *nptr);
int				init_t_philosopher(
	t_philosopher *philosopher,
	unsigned num,
	t_params params);
int				validate_argv(int argc, char **argv);
long			convert_to_milliseconds(struct timeval time);
size_t			ft_strlen(const char *str);
void			ft_putstr(char *s);
long			diff_time(struct timeval end, struct timeval start);
int				set_t_params(t_params *params, int argc, char **argv);
char			*ft_ltoa(long n);
char			*ft_strjoin(char const *s1, char const *s2);
void			write_message(long time, int phs_num, char *message);

#endif
