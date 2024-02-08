#include "common.h"

#include <math.h>
#include <pthread.h>

#define N_RUNNERS 3
#define EPSILON 1e-5

static double mean, median, mode;

struct stats_args
{
    int n_args;
    double *arg_values;
};

void *mean_runner(void *param);
void *median_runner(void *param);
void *mode_runner(void *param);

int main(int argc, char **argv)
{
    double *arg_values;
    int i;

    struct stats_args runner_args; // shared args for each runner
    pthread_t tids[N_RUNNERS];
    pthread_attr_t attrs[N_RUNNERS];
    void *(*runners[N_RUNNERS])(void *) = {mean_runner, median_runner, mode_runner};

    if (argc == 1)
    {
        DIE("usage: ./stats [list of numbers]");
    }

    arg_values = (double *)malloc(sizeof(double) * (argc - 1));
    for (i = 1; i < argc; i++)
    {
        arg_values[i - 1] = atof(argv[i]);
    }

    runner_args.n_args = argc - 1;
    runner_args.arg_values = arg_values;

    for (int i = 0; i < N_RUNNERS; i++)
    {
        pthread_attr_init(&attrs[i]);
        pthread_create(&tids[i], &attrs[i], runners[i], &runner_args);
    }

    printf("joining threads...\n");
    for (int i = 0; i < N_RUNNERS; i++)
    {
        pthread_join(tids[i], NULL);
    }
    printf("done joining threads\n");

    printf("mean = %f, median = %f, mode = %f\n", mean, median, mode);

    free(arg_values);

    return 0;
}

void *mean_runner(void *param)
{
    struct stats_args *runner_args = (struct stats_args *)param;

    double sum = 0.0;
    for (int i = 0; i < runner_args->n_args; i++)
    {
        sum += runner_args->arg_values[i];
    }
    mean = sum / runner_args->n_args;

    return NULL;
}

int compare(const void *a, const void *b)
{
    double double_a = *((double *)a);
    double double_b = *((double *)b);
    return (double_a > double_b) - (double_a < double_b);
}

/**
 * Extracts and copies arg_values from runner_args to avoid race conditions
 * (keep shared arg_values read-only).
 * Copy is dynamically allocated and must be freed later.
 *
 * Additionally, sorts copy of arg_values before returning.
 */
double *copy_and_sort_arg_values(struct stats_args *runner_args)
{
    double *arg_values_cpy = (double *)malloc(sizeof(double) * runner_args->n_args);
    for (int i = 0; i < runner_args->n_args; i++)
    {
        arg_values_cpy[i] = runner_args->arg_values[i];
    }
    qsort(arg_values_cpy, runner_args->n_args, sizeof(double), compare);
    return arg_values_cpy;
}

void *median_runner(void *param)
{
    struct stats_args *runner_args = (struct stats_args *)param;
    double *arg_values_cpy = copy_and_sort_arg_values(runner_args);

    int m = runner_args->n_args / 2;
    if (runner_args->n_args % 2 == 0)
    {
        median = (arg_values_cpy[m - 1] + arg_values_cpy[m]) / 2;
    }
    else
    {
        median = arg_values_cpy[m];
    }

    free(arg_values_cpy);
    return NULL;
}

void *mode_runner(void *param)
{
    struct stats_args *runner_args = (struct stats_args *)param;
    double *arg_values_cpy = copy_and_sort_arg_values(runner_args);

    mode = arg_values_cpy[0];
    int max_streak = 1, cur_streak = 1;
    for (int i = 1; i < runner_args->n_args; i++)
    {
        if (fabs(arg_values_cpy[i] - arg_values_cpy[i - 1]) > EPSILON)
        {
            cur_streak = 1;
        }
        else
        {
            cur_streak++;
            if (cur_streak > max_streak)
            {
                max_streak = cur_streak;
                mode = arg_values_cpy[i - 1];
            }
        }
    }

    free(arg_values_cpy);
    return NULL;
}