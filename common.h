#ifndef COMMON_INCLUDE_FILE
#define COMMON_INCLUDE_FILE

#include <stdio.h>
#include <stdlib.h>
#define DIE(msg)            \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    }

#endif