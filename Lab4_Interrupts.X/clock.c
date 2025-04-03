/* 
 * File:   clock.c
 * Author: Sebastian Moosbauer
 *
 * Created on April 2, 2025, 5:58 PM
 */

#include <clock.h>

void add_ms_to_watch(clock_t* const clock, unsigned int milliseconds){
    clock->milliseconds += milliseconds;
    if (clock->milliseconds >= 1000) {
        clock->milliseconds -= 1000;
        ++clock->seconds;
    }

    if (clock->seconds >= 60) {
        clock->seconds -= 60;
        ++clock->minutes;
    }

    if (clock->minutes >= 60) {
        clock->minutes -= 60;
        ++clock->hours;
    }
}

void add_sec_to_watch(clock_t* const clock, unsigned int seconds){
    clock->seconds += seconds;
    if (clock->seconds >= 60) {
        clock->seconds -= 60;
        ++clock->minutes;
    }

    if (clock->minutes >= 60) {
        clock->minutes -= 60;
        ++clock->hours;
    }
}
