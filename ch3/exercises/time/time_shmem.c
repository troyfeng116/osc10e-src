#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

int main(int argc, char **argv)
{
    const int SIZE = sizeof(struct timeval);
    const char *name = "time_shmem";

    int shared_fd;
    char *null_term_argv[argc];
    struct timeval *mapped_timeval_ptr;
    pid_t pid;

    shared_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shared_fd == -1)
    {
        DIE("shm_open failed");
    }

    if (ftruncate(shared_fd, SIZE) == -1)
    {
        DIE("ftruncate failed");
    }

    mapped_timeval_ptr = (struct timeval *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (mapped_timeval_ptr == (void *)-1)
    {
        DIE("mmap failed");
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
        if (gettimeofday(mapped_timeval_ptr, NULL) == -1)
        {
            DIE("gettimeofday failed");
        }

        execvp(null_term_argv[0], null_term_argv);

        DIE("execvp failed");
    }
    else
    {
        int wait_status;
        struct timeval current_timeval;
        long start_time, end_time;

        if (waitpid(pid, &wait_status, 0) == -1)
        {
            DIE("waitpid failed");
        }

        if (gettimeofday(&current_timeval, NULL) == -1)
        {
            DIE("gettimeofday failed");
        }

        start_time = mapped_timeval_ptr->tv_usec;
        end_time = current_timeval.tv_usec;
        printf("Elapsed time: %fs\n", (end_time - start_time) / 1e6);

        close(shared_fd);
    }

    exit(EXIT_SUCCESS);
}
