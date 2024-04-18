#include <errno.h>
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define SEM_NAME "/sem_example"

int main(void)
{
	sem_t * sem;

	sem = sem_open(SEM_NAME, O_RDWR | O_CREAT, 0666, 1);

	if (sem == SEM_FAILED) {
		perror(SEM_NAME);
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "[%d] ouverture du sémaphore %s\n", getpid(), SEM_NAME);
	}


	for (int i = 0; i < 3; i++) {
		fprintf(stdout, "[%d] attente sémaphore \n", getpid());
		sem_wait(sem);
		fprintf(stdout, "[%d] tient le sémaphore \n", getpid());
		sleep(3);
		fprintf(stdout, "[%d] libère le sémaphore \n", getpid());
		sem_post(sem);
		sleep(2);
	}

	if (sem_close(sem) != 0) {
		perror(SEM_NAME);
	}
	if (sem_unlink(SEM_NAME) != 0) {
		perror(SEM_NAME);
	}

	return EXIT_SUCCESS;
}

