#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>           /* pour les constantes O_* */
#include <sys/stat.h>        /* pour les modes d'ouverture */
#include <mqueue.h>

#include "protocol.h"

#define MQ_MSG_MAX (10)

#define MQ_MSG_SIZE (48)


int main()
{
	mqd_t mq;
	struct mq_attr attr;

	// configuration de la file de message
	attr.mq_maxmsg = MQ_MSG_MAX;
	attr.mq_msgsize = MQ_MSG_SIZE;

	// création/ouverture de la file de message
	if ((mq = mq_open(MQ_NAME, O_RDONLY | O_CREAT, 0644, &attr)) == -1) {
		perror("mq_open()");
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "ouverture en lecture de %s\n", MQ_NAME);
	fflush(stdout);

	char buffer[MQ_MSG_SIZE];

	/* reception et affichage des messages, jusqu'à réception du message MQ_MSG_QUIT */
	do {
		if (mq_receive(mq, buffer, MQ_MSG_SIZE, NULL) == -1) {
			perror("mq_receive()");
			mq_close(mq);
			mq_unlink(MQ_NAME);
			exit(EXIT_FAILURE);
		}

		fprintf(stdout, "> %s", buffer);

	} while (strncmp(buffer, MQ_MSG_QUIT, strlen(MQ_MSG_QUIT)));

	// fermeture de la file de message
	mq_close(mq);

	// destruction de la file de message
	mq_unlink(MQ_NAME);

	return EXIT_SUCCESS;
}
