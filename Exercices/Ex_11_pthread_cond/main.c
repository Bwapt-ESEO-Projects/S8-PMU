#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * thread_1_fct(void * arg);

int main(void)
{
	pthread_t thread_1;

	fprintf(stdout, "Process PID %d\n", getpid());

	if (pthread_create(&thread_1, NULL, thread_1_fct, NULL) != 0) {
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "Thread main attend la condition\n");

	pthread_mutex_lock(&cond_mutex);
	pthread_cond_wait(&cond, &cond_mutex);
	pthread_mutex_unlock(&cond_mutex);

	fprintf(stdout, "Thread main a reçu la condition\n");

	fprintf(stdout, "Thread main attend Thread 1\n");
	pthread_join(thread_1, NULL);

	pthread_mutex_destroy(&cond_mutex);
	pthread_cond_destroy(&cond);

	fprintf(stdout, "Thread main sortie\n");
	return EXIT_SUCCESS;
}

void * thread_1_fct(void * arg)
{
	for (int i = 0; i < 3; i++) {
		fprintf(stdout, "Thread 1\n");
		sleep(1);
	}
	
	fprintf(stdout, "Thread 1 signale la condition\n");

	pthread_mutex_lock(&cond_mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&cond_mutex);

	for (int i = 0; i < 3; i++) {
		fprintf(stdout, "Thread 1\n");
		sleep(1);
	}

	return NULL;
}


