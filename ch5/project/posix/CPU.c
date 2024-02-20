/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

// run this task for the specified time slice
void run(Task *task, int slice)
{
    printf("[CPU] running task = [%s] [%d] [%d] for %d units.\n", task->name, task->priority, task->burst, slice);

    task->burst -= slice;
    if (task->burst < 0)
    {
        task->burst = 0;
    }
}
