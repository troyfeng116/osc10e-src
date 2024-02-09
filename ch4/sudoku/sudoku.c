#include "common.h"

#include <pthread.h>

#define BOARD_SIZE 9

static int SUDOKU[BOARD_SIZE][BOARD_SIZE] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 9},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}};

static int row_results[BOARD_SIZE];
static int col_results[BOARD_SIZE];
static int square_results[BOARD_SIZE];

void *check_row(void *args);
void *check_col(void *args);
void *check_square(void *args);

struct square_args
{
    int r;
    int c;
};

int main(int argc, char **argv)
{
    struct square_args square_args[BOARD_SIZE];
    int row_col_args[BOARD_SIZE];
    pthread_t row_tids[BOARD_SIZE], col_tids[BOARD_SIZE], square_tids[BOARD_SIZE];
    pthread_attr_t row_attrs[BOARD_SIZE], col_attrs[BOARD_SIZE], square_attrs[BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        row_col_args[i] = i;
    }

    // row workers
    for (int r = 0; r < BOARD_SIZE; r++)
    {
        pthread_attr_init(&row_attrs[r]);
        pthread_create(&row_tids[r], &row_attrs[r], check_row, &row_col_args[r]);
    }

    // col workers
    for (int c = 0; c < BOARD_SIZE; c++)
    {
        pthread_attr_init(&col_attrs[c]);
        pthread_create(&col_tids[c], &col_attrs[c], check_col, &row_col_args[c]);
    }

    // square workers
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int r = (i / 3) * 3, c = (i % 3) * 3;
        square_args[i].r = r;
        square_args[i].c = c;
        pthread_attr_init(&square_attrs[i]);
        pthread_create(&square_tids[i], &square_attrs[i], check_square, &square_args[i]);
    }

    // join all workers
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        pthread_join(row_tids[i], NULL);
        pthread_join(col_tids[i], NULL);
        pthread_join(square_tids[i], NULL);
    }

    // validate results
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (row_results[i] > 0)
        {
            printf("row %d missing value %d\n", i, row_results[i]);
        }
        if (col_results[i] > 0)
        {
            printf("col %d missing value %d\n", i, col_results[i]);
        }
        if (square_results[i] > 0)
        {
            printf("square %d missing value %d\n", i, square_results[i]);
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
    row_results[r] = check_found_arr(found);
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
    col_results[c] = check_found_arr(found);
    return NULL;
}

/**
 * Check 3x3 square with top-left corner at (args->r, args->c).
 */
void *check_square(void *args)
{
    struct square_args *square_args = (struct square_args *)args;
    int r = square_args->r, c = square_args->c;
    int found[BOARD_SIZE];
    for (int i = r; i < r + 3; i++)
    {
        for (int j = c; j < c + 3; j++)
        {
            found[SUDOKU[i][j] - 1] = 1;
        }
    }
    square_results[(r / 3) * 3 + c / 3] = check_found_arr(found);
    return NULL;
}