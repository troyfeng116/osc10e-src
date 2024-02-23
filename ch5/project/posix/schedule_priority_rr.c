#include "cpu.h"
#include "evaluate.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>

// comparator: order nodes by task priority ascending, tiebreak by tid (FIFO)
int comp_nodes_prio_rr(const void *a, const void *b)
{
    Task *a_task = (*(struct node **)a)->task;
    Task *b_task = (*(struct node **)b)->task;
    return a_task->priority == b_task->priority ? a_task->tid - b_task->tid : a_task->priority - b_task->priority;
}

// run tasks in queue round robin until all bursts completed
// queue will be empty, but queue/polled nodes are NOT destroyed
void run_rr(Queue *q)
{
    struct node *polled_node;
    Task *polled_task;
    int slice;

    while ((polled_node = poll_queue(q)) != NULL)
    {
        polled_task = polled_node->task;
        slice = polled_task->_remaining_burst > QUANTUM ? QUANTUM : polled_task->_remaining_burst;

        run(polled_task, slice);

        if (polled_task->_remaining_burst > 0)
        {
            push_queue(q, polled_node);
        }
    }
}

// invoke the scheduler
void schedule()
{
    int sz = TASK_QUEUE->sz;
    struct node **node_arr = drain_to_array(TASK_QUEUE);

    // highest prio (lowest prio number) job first
    qsort(node_arr, sz, sizeof(struct node *), comp_nodes_prio_rr);

    int r = 0;
    while (r < sz)
    {
        int l = r;
        int prio_class = node_arr[l]->task->priority;
        while (r < sz && node_arr[r]->task->priority == prio_class)
        {
            r++;
        }

        Queue *prio_class_q = create_queue();
        for (int i = l; i < r; i++)
        {
            push_queue(prio_class_q, node_arr[i]);
        }

        printf("[schedule] run %d tasks in priority class %d rr\n", prio_class_q->sz, prio_class);
        run_rr(prio_class_q);
        destroy_queue(prio_class_q);
    }

    evaluate_and_report(node_arr, sz);

    destroy_node_arr(node_arr, sz);
    destroy_queue(TASK_QUEUE);
}