#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

#define NB_PERE 5
#define NB_FILS 100

int main(void)
{
    pid_t pid_fils;

    for (int i = 0; i < NB_PERE; i++)
    {
        pid_fils = fork();

        if (pid_fils == -1)
        {
            fprintf(stderr, "fork() impossible, errno = %d\n", errno);
            return 1;
        }
        else if (pid_fils != 0)
        {
            /* processus père */
            fprintf(stdout, "Père : PID = %ld, PPID = %ld, PID fils = %ld\n", (long)getpid(), (long)getppid(), (long)pid_fils);
        }
        else {
            for(int j = 0; j < NB_FILS; j++)
                printf(" - Je suis le fils %d\n", i);
            break;
        }
    }
}