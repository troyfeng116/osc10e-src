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

#define EXIT_CMD "q"

#define HISTORY_CMD "!!"
#define HISTORY_ERR_MSG "No commands in history."

void print_args(char *args[], int n_args)
{
    for (int i = 0; i < n_args - 1; i++)
    {
        printf("%s ", args[i]);
    }
    printf("%s\n", args[n_args - 1]);
}

void destroy_args(char *args[], int max_args)
{
    for (int i = 0; i < max_args; i++)
    {
        if (args[i] != NULL)
        {
            free(args[i]);
        }
    }
}

/**
 * Read command from line of stdin; populate `args` array; return list of args, with terminating NULL.
 * Arg strings are heap-allocated and must be freed.
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

    // null-terminate args
    args[args_idx] = NULL;
    return args_idx;
}

/**
 * Fork child process, execute `args` with `execvp`.
 * Parent process waits unless backgrounded.
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

    if (pid == 0)
    {
        // if backgrounded, remove background arg in child process before exec
        if (is_backgrounded)
        {
            args[n_args - 1] = NULL;
        }
        execvp(args[0], args);
        printf("command %s failed with status code %d\n", args[0], errno);
        exit(errno);
    }
    else
    {
        // wait for child to join if not backgrounded
        if (!is_backgrounded)
        {
            if (waitpid(0, &status, 0) == -1)
            {
                DIE("waidpid failed");
            }
        }
    }
}

void handle_args(char *args[], int n_args, int *n_run)
{
    fork_and_execute(args, n_args);
    (*n_run)++;
}

int main(void)
{
    int max_args = MAX_LINE / 2 + 1;
    char *args[max_args]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;

    // handle history command
    int n_run = 0;
    int prev_n_args;
    char *prev_args[max_args];

    for (int i = 0; i < max_args; i++)
    {
        args[i] = NULL;
        prev_args[i] = NULL;
    }

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        int n_args = read_args_from_stdin(args, max_args);

        if (n_args == 0)
        {
            continue;
        }

        // handle exit command
        if (n_args == 1 && strcmp(args[0], EXIT_CMD) == 0)
        {
            break;
        }

        // handle history command
        if (n_args == 1 && strcmp(args[0], HISTORY_CMD) == 0)
        {
            if (n_run == 0)
            {
                printf("%s\n", HISTORY_ERR_MSG);
            }
            else
            {
                print_args(prev_args, prev_n_args);
                handle_args(prev_args, prev_n_args, &n_run);
            }
            destroy_args(args, max_args);
            continue;
        }

        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */

        handle_args(args, n_args, &n_run);

        // store prev args
        destroy_args(prev_args, max_args);
        prev_n_args = n_args;
        for (int i = 0; i < max_args; i++)
        {
            prev_args[i] = args[i];
        }
    }

    destroy_args(args, max_args);
    destroy_args(prev_args, max_args);

    return 0;
}
