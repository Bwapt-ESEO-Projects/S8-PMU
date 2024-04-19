//
// \file main.c
//
// \brief Source code example for Alambix programming.
//

#include "alambix.h"
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <mqueue.h>
#include <string.h>

pthread_mutex_t rdv = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;
struct mq_attr attr;
mqd_t mq;

void *alambix_thread_client_fct(void *arg);
void *alambix_thread_waiter_fct(void *arg);
void *alambix_thread_bartender_fct(void *);

#define MQ_MSG_SIZE (48)
#define MQ_MSG_MAX (10)

void alambix_init()
{

	if (pthread_mutex_init(&rdv, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_mutex_init\n");
		exit(EXIT_FAILURE);
	}

	if (sem_init(&sem, 0, 0) != 0)
	{
		fprintf(stderr, "erreur sem_init\n");
		exit(EXIT_FAILURE);
	}

	attr.mq_maxmsg = MQ_MSG_MAX;
	attr.mq_msgsize = MQ_MSG_SIZE;

	mq_unlink(ALAMBIX_BARTENDER_MQ);
	if ((mq = mq_open(ALAMBIX_BARTENDER_MQ, O_RDWR | O_CREAT, 0644, &attr)) == -1)
	{
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{

	alambix_open();

	sem_destroy(&sem);
	pthread_mutex_destroy(&rdv);
	mq_close(mq);
	mq_unlink(ALAMBIX_BARTENDER_MQ);
	return alambix_close();
}

void alambix_help()
{

	pid_t pid_fils;

	pid_fils = fork();

	if (pid_fils == 0)
	{
		char *path;
		char *app;

		app = "firefox ";

		path = alambix_help_html();

		execl("/usr/bin/firefox", app, path, NULL);

		fprintf(stderr, "exec have failed\n");
		fflush(stderr);
	}
}

void alambix_start()
{
	if (pthread_create(&alambix_waiter_thread, NULL, alambix_thread_waiter_fct, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create_waiter\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&alambix_client1_thread, NULL, alambix_thread_client_fct, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create_client\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&alambix_client0_thread, NULL, alambix_thread_client_fct, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create_client\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&alambix_bartender_thread, NULL, alambix_thread_bartender_fct, NULL) != 0)
	{
		fprintf(stderr, "erreur pthread_create_bartender\n");
		exit(EXIT_FAILURE);
	}

	pthread_detach(alambix_bartender_thread);
	pthread_detach(alambix_waiter_thread);
	pthread_detach(alambix_client0_thread);
	pthread_detach(alambix_client1_thread);
}

void *alambix_thread_client_fct(void *arg)
{
	char *drink;
	while ((drink = alambix_choose_drink()) != NULL)
	{
		pthread_mutex_lock(&rdv);
		alambix_order_drink(drink);
		pthread_mutex_unlock(&rdv);
	}
	sem_post(&sem);
}

void *alambix_thread_waiter_fct(void *arg)
{
	char *ordered_drink;

	sem_wait(&sem);
	sem_wait(&sem);
	alambix_take_order();

	while ((ordered_drink = alambix_get_ordered_drink()) != NULL)
	{
		if (mq_send(mq, ordered_drink, strlen(ordered_drink) + 1, 0) == -1)
		{
			perror("mq_send()");
			exit(EXIT_FAILURE);
		}
	}
}

void *alambix_thread_bartender_fct(void *)
{

	char ordered_drink[MQ_MSG_SIZE];
	do
	{
		if (mq_receive(mq, ordered_drink, MQ_MSG_SIZE, NULL) == -1)
		{
			perror("mq_receive()");
			exit(EXIT_FAILURE);
		}
	} while (alambix_has_ordered_drink());
}