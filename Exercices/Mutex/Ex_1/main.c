#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2

#define NUM_INCREMENTS 100000

int counter = 0;
pthread_mutex_t mutex;


void *incrementer(void *arg);


int main(void) {
    pthread_t threads[NUM_THREADS];
    int i;

    if (pthread_mutex_init(&mutex, NULL) != 0) {

        fprintf(stderr, "erreur pthread_mutex_init\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, incrementer, NULL) != 0) {
            fprintf(stderr, "erreur pthread_create\n");
            exit(EXIT_FAILURE);
        }
    }


    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "erreur pthread_join\n");
            exit(EXIT_FAILURE);
        }
    }


    printf("Valeur du compteur partagé: %d\n", counter);

    pthread_mutex_destroy(&mutex);

    pthread_exit(EXIT_SUCCESS);
}


void *incrementer(void *arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

