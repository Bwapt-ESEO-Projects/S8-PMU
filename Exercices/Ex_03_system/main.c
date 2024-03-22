#include <stdio.h>

#include <stdlib.h>

#include <errno.h>


int main(void)
{
	char * commande = "xclock -digital";

	if (system(commande) == -1) {
		fprintf(stderr, "erreur system(), errno %d\n", errno);
		return 1;
	}

	fprintf(stdout, "%s a été lancé, et maintenant fermé.\n", commande);
	fflush(stdout);

	return 0;
}


