/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:08:26 by user42            #+#    #+#             */
/*   Updated: 2020/11/18 09:39:13 by user42           ###   ########.fr       */
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
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>

# define SUCCESS 0
# define ERROR 1

# define EXIT_DIED 0
# define EXIT_END 1

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DIED 4
# define START 5
# define END 6
# define MONITORSTART 7
# define MONITORDIED 8

# define SEMNAME_FORK "philo_two_fork"
# define SEMNAME_DIED "philo_two_died"
# define SEMNAME_PARAMS "philo_two_params"
# define SEMNAME_EXIT "philo_two_exit"
# define SEMNAME_BOTH_FORKS "philo_two_both_forks"

typedef struct	s_params
{
	int				number_of_phs;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	bool			flag_set_max_number_of_eat;
	int				max_number_of_eat;
	bool			someone_died;
	sem_t			*sem_forks;
	sem_t			*sem_died;
	sem_t			*sem_params;
	sem_t			*sem_exit_code;
	sem_t			*sem_both_forks;
	pid_t			*child_pids;
	pid_t			pid;
	int				status;	
}				t_params;

typedef struct	s_philosopher
{
	int				num;
	int				num_of_eat;
	struct timeval	start_time_to_eat;
	int				exit_code;	
	t_params		*params;
}				t_philosopher;

int				ft_atoi(const char *nptr);
int				init_t_philosopher(
					t_philosopher *philosopher,
					unsigned num,
					t_params *params);
int				validate_argv(int argc, char **argv);
long			convert_to_milliseconds(struct timeval time);
size_t			ft_strlen(const char *str);
void			ft_putstr(char *s);
long			diff_time(struct timeval end, struct timeval start);
int				set_t_params(t_params *params, int argc, char **argv);
char			*ft_ltoa(long n);
char			*ft_strjoin(char const *s1, char const *s2);
void			write_message(long time, int message_kind, t_philosopher *phs);
bool			death_confirmation(struct timeval now, t_philosopher *phs);
void			*died_check(void *args);
void			*work(void *args);
void			work_start(t_philosopher *phs);
void			work_end(t_philosopher *phs);
void			work_eat(t_philosopher *phs);
void			free_memory_and_close_sem(t_params *params);
bool			allocation_memory_and_open_sem(t_params *params);

#endif
