#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * thread_1_fct(void * arg);
void * thread_2_fct(void * arg);

int main(void)
{
	pthread_t thread_1;
	pthread_t thread_2;

	fprintf(stdout, "Process PID %d\n", getpid());

	if (pthread_create(&thread_1, NULL, thread_1_fct, NULL) != 0) {
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&thread_2, NULL, thread_2_fct, (void *)42) != 0) {
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 6; i++) {
		fprintf(stdout, "Thread main\n");
	}

	pthread_exit(EXIT_SUCCESS);
}

void * thread_1_fct(void * arg)
{
	for (int i = 0; i < 3; i++) {
		fprintf(stdout, "Thread 1\n");
		sleep(4);
	}
	return NULL;
}

void * thread_2_fct(void * arg)
{
	for (int i = 0; i < 3; i++) {
		fprintf(stdout, "Thread 2 (argument = %ld)\n", (long)arg);
		sleep(4);
	}
	return NULL;
}


