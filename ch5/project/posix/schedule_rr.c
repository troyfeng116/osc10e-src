#include "cpu.h"
#include "evaluate.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>

// invoke the scheduler
void schedule()
{
    struct node *polled_node;
    Task *polled_task;
    int slice;
    int sz = TASK_QUEUE->sz;
    struct node **node_arr = copy_to_array(TASK_QUEUE);

    while ((polled_node = poll_queue(TASK_QUEUE)) != NULL)
    {
        polled_task = polled_node->task;
        slice = polled_task->_remaining_burst > QUANTUM ? QUANTUM : polled_task->_remaining_burst;

        run(polled_task, slice);

        if (polled_task->_remaining_burst > 0)
        {
            push_queue(TASK_QUEUE, polled_node);
        }
    }

    evaluate_and_report(node_arr, sz);

    destroy_node_arr(node_arr, sz);
    destroy_queue(TASK_QUEUE);
}