#include "evaluate.h"
#include "queue.h"
#include "run_rr.h"
#include "schedulers.h"

#include <stdio.h>

// invoke the scheduler
void schedule()
{
    int sz = TASK_QUEUE->sz;
    struct node **node_arr = copy_to_array(TASK_QUEUE);

    run_rr(TASK_QUEUE);

    evaluate_and_report(node_arr, sz);

    destroy_node_array(node_arr, sz);
    destroy_queue(TASK_QUEUE);
}