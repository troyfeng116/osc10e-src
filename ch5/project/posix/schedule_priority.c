#include "cpu.h"
#include "evaluate.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>

// comparator: order tasks by priority ascending, tiebreak by tid (FIFO)
int comp_nodes_prio(const void *a, const void *b)
{
    Task *a_task = (*(struct node **)a)->task;
    Task *b_task = (*(struct node **)b)->task;
    return a_task->priority == b_task->priority ? a_task->tid - b_task->tid : a_task->priority - b_task->priority;
}

// invoke the scheduler
void schedule()
{
    int sz = TASK_QUEUE->sz;
    struct node **node_arr = drain_to_array(TASK_QUEUE);

    // highest prio (lowest prio number) job first
    qsort(node_arr, sz, sizeof(struct node *), comp_nodes_prio);

    for (int i = 0; i < sz; i++)
    {
        run(node_arr[i]->task, node_arr[i]->task->burst);
    }

    evaluate_and_report(node_arr, sz);

    destroy_node_arr(node_arr, sz);
    destroy_queue(TASK_QUEUE);
}