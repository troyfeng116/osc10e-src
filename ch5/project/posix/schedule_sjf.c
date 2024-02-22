#include "cpu.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>

int comp_tasks_burst(const void *a, const void *b)
{
    Task **a_task = (Task **)a;
    Task **b_task = (Task **)b;
    return (*a_task)->burst - (*b_task)->burst;
}

// invoke the scheduler
void schedule()
{
    int sz = TASK_QUEUE->sz;
    Task **task_arr = drain_to_array(TASK_QUEUE);

    // shortest job first
    qsort(task_arr, sz, sizeof(Task *), comp_tasks_burst);

    for (int i = 0; i < sz; i++)
    {
        run(task_arr[i], task_arr[i]->burst);
    }

    free(task_arr);
    destroy_queue(TASK_QUEUE);
}