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
    return task;
}

void print_task(Task *task)
{
    printf("[%s] [%d] [%d]\n", task->name, task->priority, task->burst);
}