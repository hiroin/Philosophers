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
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# define SUCCESS 0
# define ERROR 1
# define SEMNAME "sem"

typedef struct	s_params
{
	int				number_of_phs;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	bool			flag_set_max_number_of_eat;
	int				max_number_of_eat;
	struct timeval	start_timestamp;
	sem_t			*sem_forks;
	sem_t			*sem_died;
}				t_params;

void	*work(void *arg)
{
	int		*i;

	i = arg;
	if (*i == 1)
		return (NULL);
	else
	{
		while (1)
			;
	}
	return (NULL);
}

int				main(int argc, char **argv)
{
	pid_t		pid;
	sem_t		*sem;
	int			i;
	int			status;
	t_params	params;
	pid_t		*child_pids;
	pthread_t	thread;

	sem_unlink(SEMNAME);
	sem = sem_open(SEMNAME, O_CREAT, 0600, 1);
	child_pids = malloc(sizeof(pid_t *) * 4);
	gettimeofday(&params.start_timestamp, NULL);
	i = 0;
	while (i < 4)
	{
		pid = fork();
		if (pid == -1)
		{
			perror(NULL);
			return (ERROR);
		}
		if (pid == 0)
		{
			//sem_wait(sem);
			printf("Child Process : %d start\n", i);
			pthread_create(&thread, NULL, work, &i);
			//pthread_join(thread, NULL);
			pthread_detach(thread);
			//sem_post(sem);
			// while (1)
			// 	;
			sleep((i + 1) * 2);
			printf("Child Process : %d end\n", i);
			exit(SUCCESS);
		}
		printf("Child Process Num : %d\n", pid);
		child_pids[i] = pid;
		i++;
	}
	//sleep(10);
	waitpid(-1 , &status, 0);
	i = 0;
	while (i < 4)
	{
		kill(child_pids[i], SIGTERM);
		printf("Child Process : %d kill\n", i++);
	}
	sem_close(sem);
	sem_unlink(SEMNAME);
	free(child_pids);
	return (SUCCESS);
}