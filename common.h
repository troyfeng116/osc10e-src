#ifndef COMMON_INCLUDE_FILE
#define COMMON_INCLUDE_FILE

#define DIE(msg)            \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    }

#endif