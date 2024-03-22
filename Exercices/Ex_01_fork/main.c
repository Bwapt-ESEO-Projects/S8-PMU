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
		sleep(1);
		return 0;
	} else {
		/* processus père */
		fprintf(stdout, "Père : PID = %ld, PPID = %ld, PID fils = %ld\n", (long)getpid(), (long)getppid(), (long)pid_fils);
		sleep(1);
		return 0;
	}
}


