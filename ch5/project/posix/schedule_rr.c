#include "cpu.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// invoke the scheduler
void schedule()
{
    struct node *polled_node;
    Task *polled_task;
    int slice;

    while ((polled_node = poll_queue(q)) != NULL)
    {
        polled_task = polled_node->task;
        slice = polled_task->burst > QUANTUM ? QUANTUM : polled_task->burst;

        run(polled_task, slice);
        // TODO: move to run?
        // polled_task->burst -= slice;

        // printf("[schedule] executing [%s] for %d, remaining task: ", polled_task->name, slice);
        // print_task(polled_task);

        if (polled_task->burst > 0)
        {
            push_queue(q, polled_node);
        }
        else
        {
            destroy_node(polled_node);
        }
    }

    destroy_queue(q);
}