#include "barrier.h"
#include "common.h"

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define N 5

struct worker_args
{
    int worker_id;
    int delay;
};

suseconds_t get_current_time_ms()
{
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    return cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000;
}

void *worker(void *args)
{
    struct worker_args *worker_args = (struct worker_args *)args;
    int worker_id = worker_args->worker_id;
    int delay = worker_args->delay;

    suseconds_t start_time_ms = get_current_time_ms();

    printf("[worker %d] sleeping for %d seconds at start time %ld ms\n", worker_id, delay, start_time_ms);
    sleep(delay);

    suseconds_t time_elapsed = get_current_time_ms() - start_time_ms;
    printf("[worker %d] arrived at barrier, %ld ms elapsed\n", worker_id, time_elapsed);
    barrier_point();

    time_elapsed = get_current_time_ms() - start_time_ms;
    printf("[worker %d] made it past barrier, %ld ms elapsed\n", worker_id, time_elapsed);

    return NULL;
}

int main()
{
    pthread_t tids[N];
    pthread_attr_t attrs[N];
    struct worker_args args[N];

    for (int i = 0; i < N; i++)
    {
        args[i].worker_id = i;
        args[i].delay = (i + 1) * 3;
        pthread_attr_init(&attrs[i]);
    }

    barrier_init(5);

    for (int i = 0; i < N; i++)
    {
        pthread_create(&tids[i], &attrs[i], worker, &args[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(tids[i], NULL);
    }

    return 0;
}