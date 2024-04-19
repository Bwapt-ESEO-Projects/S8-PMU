#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>           /* pour les constantes O_* */
#include <sys/stat.h>        /* pour les modes d'ouverture */
#include <mqueue.h>

#include "protocol.h"

int main()
{
	mqd_t mq;
	struct mq_attr attr;

	// ouverture de la file de message
	if ((mq = mq_open(MQ_NAME, O_WRONLY)) == -1) {
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "ouverture en écriture de %s\n", MQ_NAME);
	fflush(stdout);

	// récupération de la taille maximum d'un message

	if (mq_getattr(mq, &attr) == -1) {
		perror("mq_getattr()");
		mq_close(mq);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "taille max. d'un message : %ld\n", attr.mq_msgsize);
	fflush(stdout);

	char buffer[attr.mq_msgsize];

	/* envoi des messages, terminé par l'envoi du message MQ_MSG_QUIT */
	fprintf(stdout, "écrire au receveur (\"%s\" pour quitter)\n", MQ_MSG_QUIT);
	do {
		fprintf(stdout, "> ");
        	fflush(stdout);

        	memset(buffer, 0, attr.mq_msgsize);
        	fgets(buffer, attr.mq_msgsize, stdin);

		if (mq_send(mq, buffer, attr.mq_msgsize, 0) == -1) {
			perror("mq_send()");
			mq_close(mq);
			exit(EXIT_FAILURE);
		}

	} while (strncmp(buffer, MQ_MSG_QUIT, strlen(MQ_MSG_QUIT)));

	// fermeture de la file de message
	mq_close(mq);

	return EXIT_SUCCESS;
}
