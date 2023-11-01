#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define DIE(msg)            \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    }

int main(int argc, char **argv)
{
    const int SIZE = 4096;
    const char *name = "time_shmem";

    int shared_fd;
    char *null_term_argv[argc];
    char *mapped_ptr;
    struct timeval current;
    pid_t pid;
    int wait_status;
    long start_time, end_time;

    shared_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shared_fd == -1)
    {
        DIE("shm_open failed");
    }

    if (ftruncate(shared_fd, SIZE) < 0)
    {
        DIE("ftruncate failed");
    }

    mapped_ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (mapped_ptr == (void *)-1)
    {
        DIE("mmap failed");
    }

    for (int i = 1; i < argc; i++)
    {
        null_term_argv[i - 1] = argv[i];
    }
    null_term_argv[argc - 1] = NULL;

    pid = fork();
    if (pid < 0)
    {
        DIE("fork failed");
    }

    if (pid == 0)
    {
        if (gettimeofday(&current, NULL) < 0)
        {
            DIE("gettimeofday failed");
        }

        sprintf(mapped_ptr, "%ld", current.tv_usec);
        execvp(null_term_argv[0], null_term_argv);

        DIE("execvp failed");
    }
    else
    {
        if (waitpid(pid, &wait_status, 0) < 0)
        {
            DIE("waitpid failed");
        }

        if (gettimeofday(&current, NULL) < 0)
        {
            DIE("gettimeofday failed");
        }

        start_time = atol(mapped_ptr);
        end_time = current.tv_usec;
        printf("Elapsed time: %fs\n", (end_time - start_time) / 1e6);

        close(shared_fd);
    }

    exit(EXIT_SUCCESS);
}