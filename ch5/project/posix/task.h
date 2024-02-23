/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task
{
    char *name;
    int tid;
    int priority;
    int burst;

    // internal fields
    int _remaining_burst;
    int _response_time;     // -1 indicates no response yet
    int _last_preempt_time; // used to compute waiting time
    int _wait_time;
    int _completion_time; // -1 indicates incomplete
} Task;

Task *create_task(char *name, int tid, int priority, int burst);
void print_task(Task *task);
void destroy_task(Task *task);

#endif
