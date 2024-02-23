/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

static int TIME = 0;

// run this task for the specified time slice
void run(Task *task, int slice)
{
    int start_time = TIME;
    printf("[CPU] TIME=%d : running task = [%s] tid=[%d] prio=[%d] burst=[%d] remaining=[%d] for %d units.\n",
           TIME, task->name, task->tid, task->priority, task->burst, task->_remaining_burst, slice);

    slice = (task->_remaining_burst <= slice) ? task->_remaining_burst : slice;
    task->_remaining_burst -= slice;
    TIME += slice;

    if (task->_response_time == -1)
    {
        task->_response_time = start_time;
    }
    if (task->_remaining_burst == 0)
    {
        task->_completion_time = TIME;
    }
    task->_wait_time += start_time - task->_last_preempt_time;
    task->_last_preempt_time = TIME;
}
