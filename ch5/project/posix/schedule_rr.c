#include "cpu.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>

// invoke the scheduler
void schedule()
{
    struct node *polled_node;
    Task *polled_task;
    int slice;

    while ((polled_node = poll_queue(TASK_QUEUE)) != NULL)
    {
        polled_task = polled_node->task;
        slice = polled_task->burst > QUANTUM ? QUANTUM : polled_task->burst;

        run(polled_task, slice);

        if (polled_task->burst > 0)
        {
            push_queue(TASK_QUEUE, polled_node);
        }
        else
        {
            destroy_node(polled_node);
        }
    }

    destroy_queue(TASK_QUEUE);
}