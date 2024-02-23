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
    int sz = TASK_QUEUE->sz;
    struct node **node_arr = copy_to_array(TASK_QUEUE);

    while ((polled_node = poll_queue(TASK_QUEUE)) != NULL)
    {
        polled_task = polled_node->task;

        run(polled_task, polled_task->burst);
    }

    evaluate_and_report(node_arr, sz);

    destroy_node_array(node_arr, sz);
    destroy_queue(TASK_QUEUE);
}