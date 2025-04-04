# Lab4_Interrupts

Last time in the laboratory we deepened our knowledge about timers and the possibility to passively evaluate them. 
Therefore, we configured Timer0, Timer1, and CCP1 to set flags after time x. Using only a timer has the disadvantage
that we cannot easily achieve precise timings. Thus, we used the Capture Compare Module. Using the CCP increased the
accuracy of the timing, but due to the cyclic execution of the main code we stil had some very minor delays in our
evaluation.
Today, we will deepen our understanding of interrupts and how to use them to overcome those known limitations.

> [!TIP]
> In case you need the manual for Git again, click [here](https://github.com/MicrocontrollerApplications/Utilities/blob/main/git.md)
> If you need the development board's schematic, click [here](https://raw.githubusercontent.com/MicrocontrollerApplications/Lab2_InputOutput/refs/heads/main/images/uCquick_Board_2018_01.svg)
> The latest datasheet can be found [here](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/PIC18%28L%29F2X-4XK22-Data-Sheet-40001412H.pdf)

## Relevant registers
For today's laboratory we will need the previously used registers to configure our Oscialltor, I/O pins, Timers, CCP Module, and the
registers introduced in the lecture. All those registers are listed below. Please check them to ensure you are aware
of all relevant registers for today.
<details>
<summary><strong>Oscillator</strong></summary>
<table>
<tr><th align="left">OSCCON</th><td>is used to set the frequency (IRCF)</td></tr>
</table>
</details>
<details>
<summary><strong>I/O</strong></summary>
<table>
<tr><th align="left">TRISB</th><td> sets the direction of a pin: "0" = output, "1" = input</td></tr>
<tr><th align="left">PORTB</th><td> reads the logic level at the pin: "0" = 0-0.8 V, "1" = Vdd (1.6-3.3 V or 2.0-5.0 V for TTL inputs)</td></tr>
<tr><th align="left">LATB</th><td> set the output voltage of a pin: "0" = 0 V, "1" = Vdd (3.3 V or 5 V)</td></tr>
<tr><th align="left">ANSELB</th><td> sets a pin as digital or analog: "0" = digital, "1" = analog</td></tr>
<tr><th align="left">OSCCON</th><td> sets the oscillator freq.: 0x50 = 4 MHz, 0x30 = 1 MHz. The instruction freq. are 1 MHz and 250 kHz respectively</td></tr>
<tr><th align="left"><i>Register</i>bits.X</th><td> TRISBbits.TRISB3 or LATBbits.LATB3 or PORTBbits.RB3 is the direct access to the bit 3 via a struct (see structures in C).</td></tr>
</table>
</details>

<details>
<summary><strong>Timer X</strong></summary>
<table>
<tr><th align="left">TxCON</th><td> to configure Timerx (today x will be 0 and 1)</td></tr>
<tr><th align="left">TMRx<i>H/L</i></th><td> to set the timer to a given (initial) value</td></tr>
<tr><th align="left">INTCON</th><td> to check Timer0's interrupt (i.e. overflow) flag</td></tr>
</table>
</details>

<details>
<summary><strong>Capture Compare Module</strong></summary>
<table>
<tr><th align="left">CCPxCON</th><td>configure the Capture Compare Module</td></tr>
<tr><th align="left">CCPx<i>H/L</i></th><td> set comparison value for comparison mode of Capture Compare Module</td></tr>
<tr><th align="left">CCPTMRS0</th><td> set timer to be compared with CCPx<i>H/L</i></td></tr>
<tr><th align="left">PIR1</th><td> to check Capture Compare Module's interrupt flag</td></tr>
</table>
</details>

<details>
<summary><strong>Interrupts</strong></summary>
<table>
<tr><th align="left">INTCON</th><td>globally enable Interrupts, Peripheral Interrupts and Timer0 Interrupt. Read/Write Timer0 interrupt flag (overflow flag)</td></tr>
<tr><th align="left">INTCON2</th><td>configure interrupt behaviour for external signals (e.g. buttons)</td></tr>
<tr><th align="left">INTCON3</th><td>enable interrupts for special pins and read / write their interrupt flag</td></tr>
<tr><th align="left">PIR</th><td>contains the flags for peripheral interrupts</td></tr>
<tr><th align="left">PIE</th><td>contains the individual enable bits for the peripheral interrupts</td></tr>
</table>
</details>

## Relevant chapters of the datasheet
We will use many of the available modules of our microcontroller today. Thus, the relevant chapters of the datasheet 
for today's laboratory are listed below.
- 2.0 OSCILLATOR MODULE (WITH FAIL-SAFE CLOCK MONITOR)
- 9.0 INTERRUPTS
- 10.0 I/O PORTS
- 11.0 TIMER0 MODULE
- 12.0 TIMER1/3/5 MODULE WITH GATE CONTROL
- 14.0 CAPTURE/COMPARE/PWM MODULES

## Exercise 1 - Interrupt based blinking LED
### Exercise 1.a - Implement ISR
As we will focus on interrupts within this laboratory, we will first implement the skeleton for our Interrupt Service
Routine (ISR).
Therefore, go to the end of main.c, where you should find the below shown comment block.
```c
/*
 * Exercise 1.a
 * Implement interrupt service routine. Check provided code blocks 
 * within instruction!
 */
```
This is the place within the code, where our ISR will be implemented. As told in the lecture, our ISR-function needs a 
special keyword, to tell the compiler that this function needs to be placed in a special memory location.
To implement the ISR you need the following syntax.
```c
void __interrupt(high_priority) __isr(void){
    // implement the checks here
}
```
Our first interrupt will be triggered by Timer0. Hence, we need to check if Timer0's interrupt is enabled **and** 
triggered. Find out in which registers the relevant bits, TMR0IE and TMR0IF, are contained and add the required 
if-statement checking if both are true (i.e. greater 1 in C).  
Afterward, reset the interrupt flag (TMR0IF), toggle LED2, and ***return*** from the ISR.  
Last but not least add an infinity loop containing a Nop() command at the end of your ISR. 
```c
void __interrupt(high_priority) __isr(void){
    // implement the checks here
    
    // pin the program here, in case an interrupt was triggered but not processed.
    // if your program stopped execution (e.g. LED2 is not blinking anymore), press pause and check if the program stops
    // here
    while(1){
        Nop();
    }
}
```
This is done to check if your ISR misses any interrupts.
Congratulations, you implemented your first ISR!  
But before we can deploy it to our board, we need to first configure some pins and finish configuration of Timer0.
This is done in the following exercises.

### Exercise 1.b - Configure PORTB
To let the LED blink, we need to add the required configurations. Do this in line 47, or after wherever the below comment
block can be found.
```c
/*
 * Exercise 1.b
 * Pin configuration. 
 *  - Keep in mind the two possible modes.
 *  - Check the instructions for potentially relevant input pins.
 */
```

### Exercise 1.c - Finalize Timer0's configuration


## Exercise 2
Configure Timer1 and CCP1 to trigger an interrupt every 100ms. Update the clock on interrupt.


## Exercise 3
Fix bug of wrong blink frequency


## Exercise 4 (Experts only, not part of the voting)
Add interrupt for button TL and start / stop the clock if button is pressed.