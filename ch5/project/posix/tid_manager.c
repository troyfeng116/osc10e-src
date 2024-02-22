#include "tid_manager.h"

#include <stdatomic.h>

static int tid = 1;

int get_next_tid()
{
    // TODO: thread safety
    return tid++;
}