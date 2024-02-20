/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

// run this task for the specified time slice
void run(Task *task, int slice)
{
    printf("[CPU] running task = [%s] [%d] [%d] for %d units.\n", task->name, task->priority, task->burst, slice);

    if (slice <= task->burst)
    {
        task->burst -= slice;
    }
    else
    {
        task->burst = 0;
    }
}
