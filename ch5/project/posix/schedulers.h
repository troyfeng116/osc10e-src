#include "queue.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

extern Queue *TASK_QUEUE;

// add a task to the list
void add(char *name, int priority, int burst);

// invoke the scheduler
void schedule();
