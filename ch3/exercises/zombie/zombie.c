#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid, ppid, wpid;
    int status;

    ppid = getpid();
    if (ppid == -1)
    {
        perror("getpid error");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        pid = getpid();
        if (pid == -1)
        {
            perror("[zombie] getpid error");
            exit(EXIT_FAILURE);
        }

        printf("i am zombie [%d]\n", pid);
        exit(0);
    }
    else
    {
        printf("parent [%d] delaying wait for zombie [%d]...\n", ppid, pid);
        sleep(10);
        printf("parent [%d] waiting to collect zombie [%d]...\n", ppid, pid);
        wpid = waitpid(pid, &status, 0);
        if (wpid == -1)
        {
            perror("waitpid error");
            exit(EXIT_FAILURE);
        }
        printf("parent [%d] waited on [%d] with status %d\n", pid, wpid, status);
    }

    return 0;
}