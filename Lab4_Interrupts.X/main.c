/*
 * File:   main.c
 * Author: Sebastian Moosbauer
 *
 * Created on April 2, 2025, 3:21 PM
 */


#include <xc.h>

void __init(void);
void __interrupt(high_priority) __isr(void);

void main(void) {
    __init();

    while (1) {
        Nop();
    }

    return;
}

void __init(void) {
    OSCCONbits.IRCF = 0b010; // Fosc = 500kHz
        
    ANSELB = 0;
    TRISB = 0b00000100; // TL as input; remaining pins as output
    LATB = 0;
    
    
    // Configure Timer 0 (incl. interrupt)
    
    
    // Configure Timer 1 and CCP1 (incl. interrupt)
    
    
    // Configure interrupt for TL
    
}

__interrupt(high_priority) void __isr(void){
    
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF){
        // handle Timer0 interrupt here
    }
    
    if(PIE1bits.CCP1IE && PIR1bits.CCP1IF){
        // update watch on Display
    }
    
    if(1 /*react on button press here*/){
        
    }
    
    // not sure if you missed an interrupt? Set a breakpoint on the Nop() below
    // to check if you missed something.
    while(1){
        Nop();
    }
}