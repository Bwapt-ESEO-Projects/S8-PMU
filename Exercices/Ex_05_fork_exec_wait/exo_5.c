#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <errno.h>

#define NB_FILS 10
#define NB_FILS_FILS rand()

int main(void)
{
    pid_t pid_fils;

    for (int i = 0; i < NB_FILS; i++)
    {
        pid_fils = fork();

        if (pid_fils == -1)
        {
            fprintf(stderr, "fork() impossible, errno = %d\n", errno);
            return 1;
        }
        else if (pid_fils != 0)
        {
            
        }
        
    }
}