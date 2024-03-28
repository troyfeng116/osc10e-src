/**
 * initialize barrier for `n` threads that must wait at barrier point
 */
int barrier_init(int n);

/**
 * called to indicate thread has reached barrier point
 */
int barrier_point(void);