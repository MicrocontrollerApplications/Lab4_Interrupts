/*
 * File:   main.c
 * Author: Sebastian Moosbauer
 *
 * Created on April 2, 2025, 3:21 PM
 */


#include <xc.h>

#include <LCD/GLCD_library.h>

#include "clock.h"

void __init(void);
void __interrupt() __isr(void);

static clock_t clock;
static unsigned char time_step_in_ms;

/*
 * Exercise 3.a
 * Define a global variable as a flag for clock updates
 */

void main(void) {
    __init();

    while (1) {
        Nop();
        /*
         * Exercise 3.b
         * Implement clock update
         */
    }

    return;
}

void __init(void) {
    OSCCONbits.IRCF = 0b010; // Fosc = ??
    GLCD_Init();
    GLCD_Text2Out(0, 2, "00:00.0");
    clock_init(clock);

    /*
     * Exercise 1.b
     * Pin configuration. 
     *  - Keep in mind the two possible modes.
     *  - Check the instructions for potentially relevant input pins.
     */


    /* 
     * Timer0 shall overflow after approx. 130ms (no CCP required)
     */
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    /*
     * Exercise 1.c
     * Add missing configurations for Timer0 and enable the related interrupt
     */


    time_step_in_ms = 100;
    T1CONbits.T1CKPS = 2; // PS = ?
    T1CONbits.T1SYNC = 1;
    TMR1 = 0;
    /*
     * Exercise 2.a
     * Add missing configurations for Timer1
     */


    T1CONbits.TMR1ON = 1;

    /*
     * Exercise 2.b
     * Configure CCP Module and the related interrupt
     */




    /*
     * Exercise 3.a
     * Configure an external interrupt for RB2
     */


    /*
     * Exercise 1.d
     * Enable global and peripheral interrupts
     */
     
}

/*
 * Exercise 1.a
 * Implement interrupt service routine. Check provided code blocks 
 * within instruction!
 */
