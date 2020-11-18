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

void	*work(void *arg)
{
	while (1)
		;
	return (NULL);
}

int				main(int argc, char **argv)
{
	pthread_t	thread;

	pthread_create(&thread, NULL, work, NULL);
	//pthread_join(thread, NULL);
	pthread_detach(thread);
	pthread_exit(NULL);
	return (SUCCESS);
}