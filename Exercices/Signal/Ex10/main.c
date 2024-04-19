#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

pid_t pid_fils; // Déclaration de la variable globale pour que les gestionnaires de signaux puissent y accéder

void signal_sig_s_handler(int signal)
{
    fprintf(stdout, "SIGUSR1 intercepté (PID %d)\n", getpid());
    pause(); // Mettre le processus fils en attente jusqu'à ce qu'un signal soit reçu
}

void signal_sig_r_handler(int signal)
{
    fprintf(stdout, "SIGUSR2 intercepté (PID %d)\n", getpid());
    // Rien à faire ici, la suspension est déjà levée
}

void signal_sig_q_handler(int signal)
{
    fprintf(stdout, "SIGTERM intercepté (PID %d)\n", getpid());
    exit(EXIT_SUCCESS);
}

int main(void)
{
    pid_fils = fork();

    if (pid_fils == -1)
    {
        fprintf(stderr, "fork() impossible, errno = %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (pid_fils == 0)
    {
        /* processus fils */
        fprintf(stdout, "Fils : PID = %ld, PPID = %ld\n", (long)getpid(), (long)getppid());

        // Configuration des gestionnaires de signaux pour le processus fils
        signal(SIGUSR1, signal_sig_s_handler);
        signal(SIGUSR2, signal_sig_r_handler);
        signal(SIGTERM, signal_sig_q_handler);

        while (1)
        {
            fprintf(stdout, "Fils exec.\n");
            sleep(1);
        }
    }
    else
    {
        /* processus père */
        fprintf(stdout, "Père : PID = %ld, PPID = %ld\n", (long)getpid(), (long)getppid());

        char letter;

        while (1)
        {
            fprintf(stdout, "Menu :\n");
            fprintf(stdout, "Appuyez sur 's' pour endormir le fils.\n");
            fprintf(stdout, "Appuyez sur 'r' pour redémarrer le fils.\n");
            fprintf(stdout, "Appuyez sur 'q' pour tuer le fils puis terminer le père.\n");
            scanf(" %c", &letter);

            switch (letter)
            {
            case 's':
                kill(pid_fils, SIGUSR1);
                break;
            case 'r':
                kill(pid_fils, SIGUSR2);
                break;
            case 'q':
                kill(pid_fils, SIGTERM);
                wait(NULL); // Attente de la fin du fils
                exit(EXIT_SUCCESS);
            default:
                fprintf(stdout, "Choix invalide.\n");
                break;
            }
        }
    }

    return 0; // Cette ligne est inutile car main() retourne toujours 0 implicitement
}
