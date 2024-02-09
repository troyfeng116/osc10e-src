#include "common.h"

#include <pthread.h>

#define BOARD_SIZE 9

#define ROW 0
#define COL 1
#define SQUARE 2
#define DIMS 3

static int SUDOKU[BOARD_SIZE][BOARD_SIZE] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 1, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}};

// shared global for worker threads to write to
static int results[DIMS][BOARD_SIZE];

void *check_row(void *args);
void *check_col(void *args);
void *check_square(void *args);

int main(int argc, char **argv)
{
    int idx[BOARD_SIZE];
    pthread_t tids[DIMS][BOARD_SIZE];
    pthread_attr_t attrs[DIMS][BOARD_SIZE];
    void *(*runners[DIMS])(void *);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        idx[i] = i;
    }

    runners[ROW] = check_row;
    runners[COL] = check_col;
    runners[SQUARE] = check_square;

    // start workers
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int d = 0; d < DIMS; d++)
        {
            pthread_attr_init(&attrs[d][i]);
            pthread_create(&tids[d][i], &attrs[d][i], runners[d], &idx[i]);
        }
    }

    // join all workers
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int d = 0; d < DIMS; d++)
        {
            pthread_join(tids[d][i], NULL);
        }
    }

    // validate results
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (results[ROW][i] > 0)
        {
            printf("row %d missing value %d\n", i, results[ROW][i]);
        }
        if (results[COL][i] > 0)
        {
            printf("col %d missing value %d\n", i, results[COL][i]);
        }
        if (results[SQUARE][i] > 0)
        {
            printf("square %d missing value %d\n", i, results[SQUARE][i]);
        }
    }

    return 0;
}

/**
 * Searches `found` array for missing values. Return first missing value if one exists, else 0.
 */
int check_found_arr(int found[BOARD_SIZE])
{
    for (int v = 1; v <= BOARD_SIZE; v++)
    {
        if (!found[v - 1])
        {
            return v;
        }
    }
    return 0;
}

void *check_row(void *args)
{
    int r = *(int *)args;
    int found[BOARD_SIZE];
    for (int c = 0; c < BOARD_SIZE; c++)
    {
        found[SUDOKU[r][c] - 1] = 1;
    }
    results[ROW][r] = check_found_arr(found);
    return NULL;
}

void *check_col(void *args)
{
    int c = *(int *)args;
    int found[BOARD_SIZE];
    for (int r = 0; r < BOARD_SIZE; r++)
    {
        found[SUDOKU[r][c] - 1] = 1;
    }
    results[COL][c] = check_found_arr(found);
    return NULL;
}

/**
 * Check 3x3 square with top-left corner at (args->r, args->c).
 */
void *check_square(void *args)
{
    int square_idx = *(int *)args;
    int r = (square_idx / 3) * 3, c = (square_idx % 3) * 3;
    int found[BOARD_SIZE];
    for (int i = r; i < r + 3; i++)
    {
        for (int j = c; j < c + 3; j++)
        {
            found[SUDOKU[i][j] - 1] = 1;
        }
    }
    results[SQUARE][square_idx] = check_found_arr(found);
    return NULL;
}