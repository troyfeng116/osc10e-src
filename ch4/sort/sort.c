#include "common.h"

#include <pthread.h>

struct msort_args
{
    int *vals;
    int *tmp; // shared temporary array for merge step
    int l, r;
};

struct msort_args *create_msort_args(int *vals, int *tmp, int l, int r)
{
    struct msort_args *args = (struct msort_args *)malloc(sizeof(struct msort_args));
    args->vals = vals;
    args->tmp = tmp;
    args->l = l;
    args->r = r;
    return args;
}

void merge(int *vals, int *tmp, int l, int m, int r)
{
    int i = l;
    int l_ptr = l, r_ptr = m + 1;

    while (l_ptr <= m && r_ptr <= r)
    {
        if (vals[l_ptr] < vals[r_ptr])
        {
            tmp[i++] = vals[l_ptr++];
        }
        else
        {
            tmp[i++] = vals[r_ptr++];
        }
    }

    while (l_ptr <= m)
    {
        tmp[i++] = vals[l_ptr++];
    }

    while (r_ptr <= r)
    {
        tmp[i++] = vals[r_ptr++];
    }

    for (i = l; i <= r; i++)
    {
        vals[i] = tmp[i];
    }
}

void *msort_threaded(void *args)
{
    struct msort_args *msort_args, *l_args, *r_args;
    int *vals, *tmp;
    int l, r, m;
    pthread_t l_tid, r_tid;
    pthread_attr_t l_attr, r_attr;

    msort_args = (struct msort_args *)args;
    vals = msort_args->vals;
    tmp = msort_args->tmp;
    l = msort_args->l;
    r = msort_args->r;

    if (l >= r)
    {
        return NULL;
    }

    m = l + (r - l) / 2;

    // left worker
    l_args = create_msort_args(vals, tmp, l, m);
    pthread_attr_init(&l_attr);
    pthread_create(&l_tid, &l_attr, msort_threaded, l_args);

    // right worker
    r_args = create_msort_args(vals, tmp, m + 1, r);
    pthread_attr_init(&r_attr);
    pthread_create(&r_tid, &r_attr, msort_threaded, r_args);

    // join workers: thread-safe (one thread accesses given index at a time, parents block)
    pthread_join(l_tid, NULL);
    pthread_join(r_tid, NULL);

    merge(vals, tmp, l, m, r);

    free(l_args);
    free(r_args);

    return NULL;
}

int main(int argc, char **argv)
{
    int *vals, *tmp;
    struct msort_args *start_args;
    pthread_t tid;
    pthread_attr_t attr;

    int n = argc - 1;

    vals = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        vals[i] = atoi(argv[i + 1]);
    }
    tmp = (int *)malloc(sizeof(int) * n);
    start_args = create_msort_args(vals, tmp, 0, n - 1);

    // thread entry point
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, msort_threaded, start_args);
    pthread_join(tid, NULL);

    // output sorted list
    printf("sorted: [");
    for (int i = 0; i < n - 1; i++)
    {
        printf("%d, ", vals[i]);
    }
    if (n > 0)
    {
        printf("%d", vals[n - 1]);
    }
    printf("]\n");

    free(vals);
    free(tmp);
    free(start_args);

    return 0;
}