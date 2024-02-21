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

    while ((polled_node = poll_queue(q)) != NULL)
    {
        polled_task = polled_node->task;

        run(polled_task, polled_task->burst);

        destroy_node(polled_node);
    }

    destroy_queue(q);
}