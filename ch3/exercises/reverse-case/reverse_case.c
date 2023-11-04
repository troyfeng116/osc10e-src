#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char **argv)
{
    const int SIZE = 1024;
    char buffer[SIZE];

    int parent_write_pipe[2], child_write_pipe[2];
    pid_t pid;

    if (argc != 2)
    {
        printf("usage: ./reverse_case <input_string>\n");
        exit(1);
    }

    if (pipe(parent_write_pipe) == -1 || pipe(child_write_pipe) == -1)
    {
        DIE("pipe failed");
    }

    if ((pid = fork()) == -1)
    {
        DIE("fork failed");
    }

    if (pid == 0)
    {
        ssize_t n_read;

        if (close(parent_write_pipe[1]) == -1 || close(child_write_pipe[0]) == -1)
        {
            DIE("[child] close failed");
        }

        if ((n_read = read(parent_write_pipe[0], buffer, SIZE)) == -1)
        {
            DIE("[child] read failed");
        }

        for (int i = 0; i < n_read; i++)
        {
            if ('a' <= buffer[i] && buffer[i] <= 'z')
            {
                buffer[i] += ('A' - 'a');
            }
            else if ('A' <= buffer[i] && buffer[i] <= 'Z')
            {
                buffer[i] += ('a' - 'A');
            }
        }

        if (write(child_write_pipe[1], buffer, SIZE) == -1)
        {
            DIE("[child] write failed");
        }

        if (close(parent_write_pipe[0]) == -1 || close(child_write_pipe[1]) == -1)
        {
            DIE("[child] close failed");
        }

        exit(0);
    }
    else
    {
        ssize_t n_read;
        int wait_status;

        if (close(parent_write_pipe[0]) == -1 || close(child_write_pipe[1]) == -1)
        {
            DIE("[parent] close failed");
        }

        if (write(parent_write_pipe[1], argv[1], SIZE) == -1)
        {
            DIE("[parent] write failed");
        }

        if ((n_read = read(child_write_pipe[0], buffer, SIZE)) == -1)
        {
            DIE("[parent] read failed");
        }

        printf("[parent] read message: %s\n", buffer);

        if (close(parent_write_pipe[1]) == -1 || close(child_write_pipe[0]) == -1)
        {
            DIE("[parent] close failed");
        }

        if (waitpid(pid, &wait_status, 0) == -1)
        {
            DIE("waitpid failed");
        }
    }

    return 0;
}