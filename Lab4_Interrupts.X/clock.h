/* 
 * File:   watch.h
 * Author: Sebastian Moosbauer
 *
 * Created on April 2, 2025, 5:52 PM
 */

#ifndef WATCH_H
#define	WATCH_H

struct clock {
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
    unsigned int milliseconds;
};
typedef struct clock clock_t;

/**
 * Initialize clock to start from 00:00:00.000 (HH:MM:SS.MS)
 * @param clock
 */
void clock_init(clock_t* clock);

/**
 * Increase time on handed over clock for given number of milliseconds.
 * !! WARNING !!
 * Do not use this function for more than 1000 milliseconds at once!
 * 
 * @param clock: pointer to clock to be modified
 * @param milliseconds: number of milliseconds to be added to given watch
 */
void add_ms_to_watch(clock_t* const clock, unsigned int milliseconds);

/**
 * Increase time on handed over clock for given number of milliseconds.
 * !! WARNING !!
 * Do not use this function for more than 60 minutes at once!
 * 
 * @param clock:  pointer to clock to be modified
 * @param seconds: number of seconds to be added to given clock
 */
void add_sec_to_watch(clock_t* const clock, unsigned int seconds);


#endif	/* WATCH_H */

