#include "cpu.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>

static Queue *q = NULL;

// add a task to the queue
void add(char *name, int priority, int burst)
{
    if (q == NULL)
    {
        q = create_queue();
    }

    struct node *node = create_node(name, -1, priority, burst);
    push_queue(q, node);

    printf("[add]: ");
    print_task(node->task);
}

int comp_tasks_prio(const void *a, const void *b)
{
    Task **a_task = (Task **)a;
    Task **b_task = (Task **)b;
    return (*a_task)->priority - (*b_task)->priority;
}

// invoke the scheduler
void schedule()
{
    int sz = q->sz;
    Task **task_arr = drain_to_array(q);

    // shortest job first
    qsort(task_arr, sz, sizeof(Task *), comp_tasks_prio);

    for (int i = 0; i < sz; i++)
    {
        run(task_arr[i], task_arr[i]->burst);
    }

    free(task_arr);
    destroy_queue(q);
}