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

void *alambix_place_order(void *arg);

void alambix_init()
{
	// TODO: Insert initialization code here.
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
		alambix_order_drink(drink);
	}
}

int main(int argc, char *argv[])
{
	alambix_open();

	// TODO: Insert cleanup code here.

	return alambix_close();
}
