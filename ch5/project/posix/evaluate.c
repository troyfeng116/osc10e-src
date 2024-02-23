#include "evaluate.h"
#include "list.h"
#include "task.h"

#include <stdio.h>

void evaluate_and_report(struct node **nodes, int n)
{
    int total_response_time = 0, total_wait_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < n; i++)
    {
        Task *task = nodes[i]->task;
        total_response_time += task->_response_time;
        total_wait_time += task->_wait_time;
        total_turnaround_time += task->_completion_time;
    }

    printf("\n======== evaluation ========\n");
    printf("avg response time: %f\n", ((float)total_response_time) / n);
    printf("avg wait time: %f\n", ((float)total_wait_time) / n);
    printf("avg turnaround time: %f\n", ((float)total_turnaround_time) / n);
}