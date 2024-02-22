#include "cpu.h"
#include "queue.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>

// comparator: order tasks by priority ascending, tiebreak by tid (FIFO)
int comp_tasks_prio_rr(const void *a, const void *b)
{
    Task **a_task = (Task **)a;
    Task **b_task = (Task **)b;
    return (*a_task)->priority == (*b_task)->priority ? (*a_task)->tid - (*b_task)->tid : (*a_task)->priority - (*b_task)->priority;
}

void run_rr(Queue *q)
{
    struct node *polled_node;
    Task *polled_task;
    int slice;

    while ((polled_node = poll_queue(q)) != NULL)
    {
        polled_task = polled_node->task;
        slice = polled_task->burst > QUANTUM ? QUANTUM : polled_task->burst;

        run(polled_task, slice);

        if (polled_task->burst > 0)
        {
            push_queue(q, polled_node);
        }
        else
        {
            destroy_node(polled_node);
        }
    }
}

// invoke the scheduler
void schedule()
{
    int sz = TASK_QUEUE->sz;
    Task **task_arr = drain_to_array(TASK_QUEUE);

    // highest prio (lowest prio number) job first
    qsort(task_arr, sz, sizeof(Task *), comp_tasks_prio_rr);

    int r = 0;
    while (r < sz)
    {
        int l = r;
        int prio_class = task_arr[l]->priority;
        while (r < sz && task_arr[r]->priority == prio_class)
        {
            r++;
        }

        Queue *prio_class_q = create_queue();
        for (int i = l; i < r; i++)
        {
            Task *task = task_arr[i];
            push_queue(prio_class_q, create_node(task->name, task->tid, task->priority, task->burst));
            free(task); // do not free task name field still in use
        }

        printf("[schedule] run %d tasks in priority class %d rr\n", prio_class_q->sz, prio_class);
        run_rr(prio_class_q);
        destroy_queue(prio_class_q);
    }

    free(task_arr);
    destroy_queue(TASK_QUEUE);
}