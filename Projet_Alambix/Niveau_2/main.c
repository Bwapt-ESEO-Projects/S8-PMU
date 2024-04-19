//
// \file main.c
//
// \brief Source code example for Alambix programming.
//

#include "alambix.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;


void *alambix_place_order(void *arg);

void *alambix_server_function();

void alambix_init()
{

	if (pthread_mutex_init(&mutex, NULL) != 0) {

        fprintf(stderr, "erreur pthread_mutex_init\n");
        exit(EXIT_FAILURE);
    }

	if (pthread_barrier_init(&barrier, NULL, 3) != 0) {

		fprintf(stderr, "erreur pthread_barrier_init\n");
		exit(EXIT_FAILURE);
	}
}

void alambix_help()
{
	pid_t pid_fils;

	pid_fils = fork();

	if (pid_fils == -1)
	{
		fprintf(stderr, "fork() impossible, errno = %d\n", errno);
	}

	if (pid_fils == 0)
	{
		/* processus fils */
		fprintf(stdout, "Fils : PID = %ld, PPID = %ld\n", (long)getpid(), (long)getppid());

		/* lancement d'un programme dans le processus fils */
		char *argv[4];

		argv[0] = "firefox";
		argv[1] = alambix_help_html();
		argv[2] = "&";
		argv[3] = (char *)NULL;

		fprintf(stdout, "Fils : lancement de %s %s %s\n", argv[0], argv[1], argv[2]);
		fflush(stdout);

		execv("/usr/bin/firefox", argv);

		fprintf(stderr, "exec have failed\n");
		fflush(stderr);
	}
	else
	{
		/* processus père */
		fprintf(stdout, "Père : PID = %ld, PPID = %ld, PID fils = %ld\n", (long)getpid(), (long)getppid(), (long)pid_fils);
	}
}

void alambix_start()
{
	if (pthread_create(&alambix_waiter_thread, NULL, alambix_server_function, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}
	pthread_detach(alambix_waiter_thread);

	if (pthread_create(&alambix_client0_thread, NULL, alambix_place_order, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}
	pthread_detach(alambix_client0_thread);

	if (pthread_create(&alambix_client1_thread, NULL, alambix_place_order, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create\n");
		exit(EXIT_FAILURE);
	}
	pthread_detach(alambix_client1_thread);
}

void *alambix_place_order(void *arg)
{
	// order all the client's chosen drinks

	char *drink;

	while ((drink = alambix_choose_drink()) != NULL)
	{
		pthread_mutex_lock(&mutex);
		alambix_order_drink(drink);
		pthread_mutex_unlock(&mutex);
	}
	pthread_barrier_wait(&barrier);
}

void *alambix_server_function()
{
	pthread_barrier_wait(&barrier);
	alambix_take_order();
}

int main(int argc, char *argv[])
{
	alambix_open();

	mq_unlink(MQ_NAME);
	pthread_barrier_destroy(&barrier);
	pthread_mutex_destroy(&mutex);

	return alambix_close();
}
