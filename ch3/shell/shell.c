/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include "common.h"

#define MAX_LINE 80 /* 80 chars per line, per command */

/**
 * Read command from line of stdin; populate `args` array; return list of args.
 */
int read_args_from_stdin(char *args[], int max_args)
{
    char buffer[MAX_LINE];
    ssize_t nread;

    nread = read(STDIN_FILENO, buffer, MAX_LINE);
    if (nread == -1)
    {
        DIE("read failed");
    }

    char ch;
    int args_idx = 0;
    ssize_t start_idx = 0, buffer_idx = 0;
    for (; buffer_idx <= nread; buffer_idx++)
    {
        if (buffer_idx >= nread || (ch = buffer[buffer_idx]) == ' ' || ch == '\n')
        {
            if (start_idx < buffer_idx)
            {
                int arg_str_len = buffer_idx - start_idx + 1;
                char *arg_str = (char *)malloc(arg_str_len);
                if (arg_str == NULL)
                {
                    DIE("malloc failed");
                }

                for (ssize_t i = start_idx; i < buffer_idx; i++)
                {
                    arg_str[i - start_idx] = buffer[i];
                }
                arg_str[arg_str_len - 1] = '\0';
                args[args_idx++] = arg_str;
            }
            start_idx = buffer_idx + 1;
        }
    }

    for (int i = args_idx; i < max_args; i++)
    {
        args[i] = NULL;
    }
    return args_idx;
}

/**
 * Fork child process,
 */
void fork_and_execute(char *args[], int n_args)
{
    int is_backgrounded;
    int pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        DIE("fork failed")
        return;
    }

    is_backgrounded = strcmp(args[n_args - 1], "&") == 0;
    if (is_backgrounded)
    {
        args[n_args - 1] = NULL;
    }

    if (pid == 0)
    {
        execvp(args[0], args);
        exit(errno);
    }
    else
    {
        // wait for child to join if not backgrounded
        if (!is_backgrounded)
        {
            waitpid(0, &status, 0);
        }
    }
}

int main(void)
{
    int max_args = MAX_LINE / 2 + 1;
    char *args[max_args]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        int n_args = read_args_from_stdin(args, max_args);

        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */

        fork_and_execute(args, n_args);
    }

    return 0;
}
