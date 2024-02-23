#include "tid_manager.h"

#include <stdatomic.h>

static int tid = 1;

int get_next_tid()
{
    return atomic_fetch_add(&tid, 1);
}