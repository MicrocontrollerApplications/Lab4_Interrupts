/*
 * File:   clock.c
 * Author: Sebastian Moosbauer
 *
 * Created on April 2, 2025, 5:58 PM
 */

#include <clock.h>

#include "LCD/GLCD_library.h"

void clock_init(clock_t clock){
    clock->hours = 0;
    clock->minutes = 0;
    clock->seconds = 0;
    clock->milliseconds = 0;
}

void print_clock(clock_t const clock){
    GLCD_Value2Out(0, 2, clock->minutes);
    GLCD_Value2Out(0, 5, clock->seconds);
    GLCD_Value2Out(0, 8, clock->milliseconds);
}

void add_ms_to_watch(clock_t const clock, unsigned int milliseconds){
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

    print_clock(clock);
}

void add_sec_to_watch(clock_t const clock, unsigned int seconds){
    clock->seconds += seconds;
    if (clock->seconds >= 60) {
        clock->seconds -= 60;
        ++clock->minutes;
    }

    if (clock->minutes >= 60) {
        clock->minutes -= 60;
        ++clock->hours;
    }

    print_clock(clock);
}
