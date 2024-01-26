/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
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
 * Extract command from buffer into allocated string (must be freed).
 * Left-inclusive.
 */
char *extract_arg(char buffer[], ssize_t from, ssize_t to)
{
    int len = to - from + 1; // extra char for null terminator
    char *arg_str = (char *)malloc(len);
    if (arg_str == NULL)
    {
        DIE("malloc failed");
    }

    for (ssize_t i = from; i < to; i++)
    {
        arg_str[i - from] = buffer[i];
    }
    arg_str[len - 1] = '\0';
    return arg_str;
}

/**
 * Read command from line of stdin; populate `args` array; return list of args, with terminating NULL.
 * Arg strings are heap-allocated and must be freed.
 */
int read_args_from_stdin(char *args[], int max_args)
{
    char buffer[MAX_LINE];
    ssize_t nread = read(STDIN_FILENO, buffer, MAX_LINE);
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
                args[args_idx++] = extract_arg(buffer, start_idx, buffer_idx);
            }
            start_idx = buffer_idx + 1;
        }
    }

    // NULL rest of args
    for (int i = args_idx; i < max_args; i++)
    {
        args[i] = NULL;
    }
    return args_idx;
}

void execute(char *args[])
{
    execvp(args[0], args);

    fprintf(stderr, "command %s failed: [%d] %s\n", args[0], errno, strerror(errno));
    exit(EXIT_FAILURE);
}

/**
 * Fork child process, execute `args` with `execvp`.
 * Wait for child to join unless backgrounded.
 */
void fork_and_execute(char *args[], int n_args, int should_background)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        DIE("fork failed")
    }

    if (pid == 0)
    {
        // if backgrounded, NULL out last (background) arg in child process before exec
        if (should_background)
        {
            args[n_args - 1] = NULL;
        }

        execute(args);
    }
    else
    {
        // wait for child to join if not backgrounded
        if (!should_background)
        {
            if (waitpid(pid, NULL, 0) == -1)
            {
                DIE("waidpid failed");
            }
        }
    }
}

/**
 * Handle redirect argument at index `redirect_idx`.
 */
void handle_redirect(char *args[], int n_args, int redirect_idx, int should_write)
{
    char *file_arg = args[redirect_idx + 1];
    int fd = open(file_arg, should_write ? O_WRONLY | O_CREAT | O_TRUNC : O_RDONLY, S_IRWXU);
    if (fd == -1)
    {
        fprintf(stderr, "unable to open file %s: [%d] %s\n", file_arg, errno, strerror(errno));
        return;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        DIE("fork failed");
    }

    if (pid == 0)
    {
        dup2(fd, should_write ? STDOUT_FILENO : STDIN_FILENO);
        args[redirect_idx] = NULL;
        execute(args);
    }
    else
    {
        if (waitpid(pid, NULL, 0) == -1)
        {
            DIE("waidpid failed");
        }
    }
}

/**
 * Handle pipe argument at index `pipe_idx`.
 */
void handle_pipe(char *args[], int n_args, int pipe_idx)
{
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1)
    {
        DIE("pipe failed");
    }

    pid_t l_pid = fork();
    if (l_pid == -1)
    {
        DIE("fork failed");
    }
    if (l_pid == 0)
    {
        // write to write end
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);

        args[pipe_idx] = NULL;
        execute(args);
    }

    pid_t r_pid = fork();
    if (r_pid == -1)
    {
        DIE("fork failed");
    }
    if (r_pid == 0)
    {
        // read from read end
        close(pipe_fds[1]);
        dup2(pipe_fds[0], STDIN_FILENO);
        close(pipe_fds[0]);

        execute(args + pipe_idx + 1);
    }

    close(pipe_fds[0]);
    close(pipe_fds[1]);

    waitpid(l_pid, NULL, 0);
    waitpid(r_pid, NULL, 0);
}

void handle_args(char *args[], int n_args, int *n_run)
{
    int is_write;
    for (int i = 0; i < n_args; i++)
    {
        // handle redirects
        if ((is_write = strcmp(args[i], ">")) == 0 || strcmp(args[i], "<") == 0)
        {
            handle_redirect(args, n_args, i, is_write == 0);
            (*n_run)++;
            return;
        }

        // handle pipes
        if (strcmp(args[i], "|") == 0)
        {
            handle_pipe(args, n_args, i);
            (*n_run)++;
            return;
        }
    }

    // no redirects/pipes -> fork + execute
    int is_backgrounded = strcmp(args[n_args - 1], "&") == 0;
    fork_and_execute(args, n_args, is_backgrounded);
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
                fprintf(stderr, "%s\n", HISTORY_ERR_MSG);
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
