#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "common.h"

int main()
{
    pid_t pid, ppid, wpid;
    int status;

    if ((ppid = getpid()) == -1)
    {
        DIE("getpid error");
    }

    if ((pid = fork()) == -1)
    {
        DIE("fork error");
    }

    if (pid == 0)
    {
        if ((pid = getpid()) == -1)
        {
            DIE("[zombie] getpid error");
        }

        printf("i am zombie [%d]\n", pid);
        exit(0);
    }
    else
    {
        printf("parent [%d] delaying wait for zombie [%d]...\n", ppid, pid);
        sleep(10);
        printf("parent [%d] waiting to collect zombie [%d]...\n", ppid, pid);
        if ((wpid = waitpid(pid, &status, 0)) == -1)
        {
            DIE("waitpid error");
        }

        printf("parent [%d] waited on [%d] with status %d\n", ppid, wpid, status);
    }

    return 0;
}