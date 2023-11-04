#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "common.h"

#define DIE(msg)            \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    }

int main(int argc, char **argv)
{
    const int SIZE = sizeof(struct timeval);

    int pipe_fds[2];
    char *null_term_argv[argc];
    struct timeval current_timeval;
    pid_t pid;

    if (argc < 2)
    {
        printf("usage: time_pipe <cmd> <...cmd_args>\n");
        exit(1);
    }

    if ((pipe(pipe_fds)) == -1)
    {
        DIE("pipe failed");
    }

    for (int i = 1; i < argc; i++)
    {
        null_term_argv[i - 1] = argv[i];
    }
    null_term_argv[argc - 1] = NULL;

    pid = fork();
    if (pid == -1)
    {
        DIE("fork failed");
    }

    if (pid == 0)
    {
        close(pipe_fds[0]);

        if (gettimeofday(&current_timeval, NULL) == -1)
        {
            DIE("gettimeofday failed");
        }

        if (write(pipe_fds[1], &current_timeval, sizeof(struct timeval)) == -1)
        {
            DIE("write failed");
        }

        close(pipe_fds[1]);

        execvp(null_term_argv[0], null_term_argv);

        DIE("execvp failed");
    }
    else
    {
        int wait_status;
        struct timeval start_timeval;
        long start_time, end_time;

        close(pipe_fds[1]);

        if (waitpid(pid, &wait_status, 0) == -1)
        {
            DIE("waitpid failed");
        }

        if (gettimeofday(&current_timeval, NULL) == -1)
        {
            DIE("gettimeofday failed");
        }

        if (read(pipe_fds[0], &start_timeval, SIZE) == -1)
        {
            DIE("read failed");
        }

        start_time = start_timeval.tv_usec;
        end_time = current_timeval.tv_usec;
        printf("Elapsed time: %fs\n", (end_time - start_time) / 1e6);

        close(pipe_fds[0]);
    }

    exit(EXIT_SUCCESS);
}
