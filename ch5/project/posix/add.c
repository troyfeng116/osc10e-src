#include "schedulers.h"
#include "queue.h"
#include "tid_manager.h"

#include <stdio.h>

Queue *TASK_QUEUE = NULL;

// add a task to the queue
void add(char *name, int priority, int burst)
{
    if (TASK_QUEUE == NULL)
    {
        TASK_QUEUE = create_queue();
    }

    int tid = get_next_tid();
    struct node *node = create_node(name, tid, priority, burst);
    push_queue(TASK_QUEUE, node);

    printf("[add]: ");
    print_task(node->task);
}