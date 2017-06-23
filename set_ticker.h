/*
 *      set_ticker.h
 *          set_ticker( number_of_milliseconds )
 *                   arranges for the interval timer to issue
 *                   SIGALRM's at regular intervals
 *          returns -1 on error, 0 for ok
 *
 *      arg in milliseconds, converted into micro seoncds
 */


int set_ticker( int n_msecs );

