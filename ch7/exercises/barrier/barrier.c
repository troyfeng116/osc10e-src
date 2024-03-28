#include "barrier.h"

#include <pthread.h>

int num_remaining; // number of threads yet to reach barrier point
pthread_mutex_t mtx;
pthread_cond_t barrier_event;

int barrier_init(int n)
{
    if (pthread_mutex_init(&mtx, NULL) != 0)
    {
        return -1;
    }
    if (pthread_cond_init(&barrier_event, NULL) != 0)
    {
        pthread_mutex_destroy(&mtx);
        return -1;
    }
    num_remaining = n;
    return 0;
}

int barrier_point(void)
{
    if (pthread_mutex_lock(&mtx) != 0)
    {
        return -1;
    }

    num_remaining--;
    while (num_remaining > 0)
    {
        if (pthread_cond_wait(&barrier_event, &mtx) != 0)
        {
            return -1;
        }
    }

    // once all threads reach barrier, signal
    if (pthread_cond_broadcast(&barrier_event) != 0)
    {
        return -1;
    }

    if (pthread_mutex_unlock(&mtx) != 0)
    {
        return -1;
    }

    return 0;
}