#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_barrier_t barrier;

void * thread_1_fct(void * arg);
void * thread_2_fct(void * arg);

int main(void)
{
	pthread_t thread_1;
	pthread_t thread_2;

	fprintf(stdout, "Process PID %d\n", getpid());

	pthread_barrier_init(&barrier, NULL, 3);

	if (pthread_create(&thread_1, NULL, thread_1_fct, NULL) != 0) {
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&thread_2, NULL, thread_2_fct, (void *)42) != 0) {
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "Thread main attend à la barrière\n");
	
	pthread_barrier_wait(&barrier);

	fprintf(stdout, "Thread main a passé la barrière\n");

	fprintf(stdout, "Thread main attend Thread 1\n");
	pthread_join(thread_1, NULL);

	fprintf(stdout, "Thread main attend Thread 2\n");
	pthread_join(thread_2, NULL);

	pthread_barrier_destroy(&barrier);

	fprintf(stdout, "Thread main sortie\n");
	return EXIT_SUCCESS;
}

void * thread_1_fct(void * arg)
{
	sleep(2);

	fprintf(stdout, "Thread 1 attend à la barrière\n");
	
	pthread_barrier_wait(&barrier);

	fprintf(stdout, "Thread 1 a passé la barrière\n");

	sleep(2);

	return NULL;
}

void * thread_2_fct(void * arg)
{
	sleep(4);

	fprintf(stdout, "Thread 2 attend à la barrière\n");

	pthread_barrier_wait(&barrier);

	fprintf(stdout, "Thread 2 a passé la barrière\n");

	sleep(4);
}


