#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>


void notify_function(union sigval val)
{	
	int value;
	
	//lecture gpio23 (broche 16) 
	value = gpiod_ctxless_get_value("0", 23, 0, "me");

	if (value < 0) {
		perror("gpiod_ctxless_get_value");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "%d\n", value);
}

int main(int argc, char *argv[])
{
	char c;
	int running = 1;
	
	//init timer -> Affichage toutes les 4 secondes
	timer_t timer;

	struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = notify_function;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &timer) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	}

	struct itimerspec itimer;

	itimer.it_interval.tv_sec = 2;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 4;
	itimer.it_value.tv_nsec = 0;


	if (timer_settime(timer, 0, &itimer, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
	printf("Taper 'q' pour quitter l'application");
	
	while(running)
	{
		c = getchar();
		if(c == 'q') running = 0;
	}
	
	return EXIT_SUCCESS;
}

