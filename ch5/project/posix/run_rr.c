#include "cpu.h"
#include "run_rr.h"
#include "task.h"

#include <stdlib.h>

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