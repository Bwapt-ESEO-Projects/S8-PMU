#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <signal.h>


void notify_function(union sigval val)
{
	char * message = (char *)(val.sival_ptr);
	time_t current_time;

	time(&current_time); 
	fprintf(stdout, "message reçu : %s, à %s\n", message, ctime(&current_time));
}

char * value = "ma valeur";

int main(void)
{
	time_t current_time;
	timer_t timer;

	struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = (void *)value;
	event.sigev_notify_function = notify_function;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &timer) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	}

	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 4;
	itimer.it_value.tv_nsec = 0;

	time(&current_time); 
	fprintf(stdout, "lancement timer à : %s\n", ctime(&current_time));

	if (timer_settime(timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 6; i++) {
		fprintf(stdout, "main travaille...\n");
		sleep(1);
	}

	fprintf(stdout, "main quitte\n");
	return 0;
}

