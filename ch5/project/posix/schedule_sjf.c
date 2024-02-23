#include "cpu.h"
#include "evaluate.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>

int comp_nodes_burst(const void *a, const void *b)
{
    Task *a_task = (*(struct node **)a)->task;
    Task *b_task = (*(struct node **)b)->task;
    return a_task->burst - b_task->burst;
}

// invoke the scheduler
void schedule()
{
    int sz = TASK_QUEUE->sz;
    struct node **node_arr = drain_to_array(TASK_QUEUE);

    // shortest job first
    qsort(node_arr, sz, sizeof(struct node *), comp_nodes_burst);

    for (int i = 0; i < sz; i++)
    {
        // run task in entirety (complete full burst)
        run(node_arr[i]->task, node_arr[i]->task->burst);
    }

    evaluate_and_report(node_arr, sz);

    destroy_node_array(node_arr, sz);
    destroy_queue(TASK_QUEUE);
}