#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAB_SIZE 5

struct Tab {
    int tableau[TAB_SIZE];
    int size;
};

void * moyenne_fct(void * arg) {
    struct Tab *tab = (struct Tab *)arg;
    int somme = 0;

    for (int i = 0; i < tab->size; i++) {
        somme += tab->tableau[i];
    }

    double *moyenne = malloc(sizeof(double));
    *moyenne = (double)somme / tab->size; 

    return moyenne;
}

int main(void) {
    srand(time(NULL));
    pthread_t thread;
    struct Tab tab;

    for (int i = 0; i < TAB_SIZE; i++) {
        tab.tableau[i] = rand() % 20;
    }
    tab.size = TAB_SIZE;
    void *moyenne;

    fprintf(stdout, "Process PID %d\n", getpid());

    if (pthread_create(&thread, NULL, moyenne_fct, &tab) != 0) {
        fprintf(stderr, "erreur pthread_create\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < tab.size; i++) {
        printf("tab[%d] = %d\n", i, tab.tableau[i]);
    }
    pthread_join(thread, &moyenne);
    printf("La moyenne est de %f\n", *((double *)moyenne));

    free(moyenne);

    return 0;
}
