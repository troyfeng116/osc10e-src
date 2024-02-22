#include "cpu.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>

// invoke the scheduler
void schedule()
{
    struct node *polled_node;
    Task *polled_task;

    while ((polled_node = poll_queue(TASK_QUEUE)) != NULL)
    {
        polled_task = polled_node->task;

        run(polled_task, polled_task->burst);

        destroy_node(polled_node);
    }

    destroy_queue(TASK_QUEUE);
}