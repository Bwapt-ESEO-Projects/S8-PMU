#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

#include <stdlib.h>

#include <sys/wait.h>

#include <signal.h>

static int run = 1;

void signal_sigint_handler(int signal)
{
	fprintf(stdout, "\nSIGINT intercepté (PID %d)\n", getpid());
	run = 0;
}

void signal_sigchld_handler(int signal)
{
	fprintf(stdout, "SIGCHLD intercepté (PID %d)\n", getpid());
	wait(NULL);
}

int main(void)
{
	/* interception du Ctrl+C pour sortie propre */
	signal(SIGINT, signal_sigint_handler);

	/* interception de SIGCHLD pour suppression de zombis à tout moment */
	signal(SIGCHLD, signal_sigchld_handler);

	pid_t pid_fils;

	pid_fils = fork();

	if (pid_fils == -1) {
		fprintf(stderr, "fork() impossible, errno = %d\n", errno);
		exit(EXIT_FAILURE);
	}

	if (pid_fils == 0) {
		/* processus fils */
		fprintf(stdout, "Fils : PID = %ld, PPID = %ld\n", (long)getpid(), (long)getppid());

		/* lancement d'un programme dans le processus fils */
		char * argv[3];

		argv[0] = "xclock";
		argv[1] = "-digital";
		argv[2] = (char *) NULL;

		fprintf(stdout, "Fils : lancement de %s %s\n", argv[0], argv[1]);
		fflush(stdout);

		execv("/usr/bin/xclock", argv);

		fprintf(stderr, "erreur exec\n");
		fflush(stderr);
		exit(EXIT_FAILURE);

	} else {
		/* processus père */
		fprintf(stdout, "Père : PID = %ld, PPID = %ld, PID fils = %ld\n", (long)getpid(), (long)getppid(), (long)pid_fils);
		
		/* exécution du traitement */
		while (run) {
			fprintf(stdout, "Père : exec.\n");
			sleep(1);
		}

		fprintf(stdout, "Père : exit\n");
		exit(EXIT_SUCCESS);
	}
}


