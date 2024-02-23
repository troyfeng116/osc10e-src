#include "task.h"

#include <stdio.h>
#include <stdlib.h>

Task *create_task(char *name, int tid, int priority, int burst)
{
    Task *task = (Task *)malloc(sizeof(Task));
    task->name = name;
    task->tid = tid;
    task->priority = priority;
    task->burst = burst;

    task->_remaining_burst = burst;
    task->_response_time = -1; // initially set to -1
    task->_last_preempt_time = 0;
    task->_wait_time = 0;
    task->_completion_time = -1; // initially set to -1
    return task;
}

void print_task(Task *task)
{
    printf("[%s] tid=[%d] prio=[%d] burst=[%d] remaining=[%d]\n",
           task->name, task->tid, task->priority, task->burst, task->_remaining_burst);
}

void destroy_task(Task *task)
{
    free(task->name);
    free(task);
}