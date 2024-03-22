#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <errno.h>


int main(void)
{
	pid_t pid_fils;

	pid_fils = fork();

	if (pid_fils == -1) {
		fprintf(stderr, "fork() impossible, errno = %d\n", errno);
		return 1;
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

		fprintf(stderr, "exec have failed\n");
		fflush(stderr);
		return 1;

	} else {
		/* processus père */
		fprintf(stdout, "Père : PID = %ld, PPID = %ld, PID fils = %ld\n", (long)getpid(), (long)getppid(), (long)pid_fils);
		
		/* laisser le temps d'observer les comportements avec 'ps fx' (objectif pédagogique) */
		sleep(15);

		return 0;
	}
}


