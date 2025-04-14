# Lab4_Interrupts

Last time in the laboratory we deepened our knowledge about timers and the possibility to passively evaluate them. 
Therefore, we configured Timer0, Timer1, and CCP1 to set flags after time x. Using only a timer has the disadvantage
that we cannot easily achieve precise timings. Thus, we used the Capture Compare Module. Using the CCP1 increased the
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
<tr><th align="left">CCPRx<i>H/L</i></th><td> set comparison value for comparison mode of Capture Compare Module</td></tr>
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
Our first interrupt will be triggered by Timer0. Hence, we need to check if Timer0's interrupt is enabled **and** triggered. Find out in which registers the relevant bits, TMR0IE and TMR0IF, are contained and add the required if-statement checking if both are true (i.e. greater 1 in C).  
Afterward, reset the interrupt flag (TMR0IF), toggle LED3, and ***return*** from the ISR.  
Last but not least add an infinity loop containing a Nop() command at the end of your ISR. 
```c
void __interrupt(high_priority) __isr(void){
    // implement the checks here
    
    // pin the program here, in case an interrupt was triggered but not processed.
    // if your program stopped execution (e.g. LED3 is not blinking anymore), press pause and check if the program stops
    // here
    while(1){
        Nop();
    }
}
```
This is done to check if your ISR misses any interrupts.  
Congratulations, you implemented your first ISR!  
> [!IMPORTANT]
> But before we can deploy it to our board, we need to first configure some pins and finish configuration of Timer0.  
> This is done in the following exercises.

### Exercise 1.b - Configure PORTB
To let the LED3 blink, we need to add the required configurations. Do this in line 47, or after wherever the below comment block can be found.
```c
/*
 * Exercise 1.b
 * Pin configuration. 
 *  - Keep in mind the two possible modes.
 *  - Check the instructions for potentially relevant input pins.
 */
```
Keep in mind, that PORTB provides two different modes for its pins and that you need to check the board's schematic to determine the required logical levels to turn the LED3 on or off.  
**Initially, all LEDs shall be turned off!**

### Exercise 1.c - Finalize Timer0's configuration
Last but not least, Timer0 needs some additional configurations. Check the current configuration and identifiy the missing configuration parameters. Timer0 shall overflow after approximately 130ms (131.072ms to be precise).
**Do not change the Timers mode to 16 Bit.** There's another solution to achieve the requried duration! Also keep in mind to check the configured oscillator frequency. ;)
> [!NOTE]
> The prescaler is already assigned but not set. Find out the necassary prescaler value! ;)
> Keep in mind, that a prescaler makes the timer slower, this must be considered in your calculations!


### Exercise 1.d - Check the functionality
As our modules are set up now, we can enable our interrupts. Please do this in line 96, below following code block. (line numbers might have changed, if you added comments.)
```C
/*
* Exercise 1.d
* Enable global and peripheral interrupts
*/
```
Now that our interrupts are enabled and our Timer, PORTB, and the ISR are good to go, it's time to check if everything is working. If not already done, connect your development board to the PC. Click on debug and check if the LED3 is blinking as expected. If yes, great! Additionally you could use the Simulator to stop the timing of the blinking LED. 
> [!NOTE]
> You can switch to Simulator Configuration using the drop-down-menu in the upper left corner of the IDE. Currently, PicKIT3 should be selected.
> Remember to open the stopwach under Window -> Debugging -> Stopwatch.

## Exercise 2 - Interrupt based clock
> [!WARNING]
> **DO NOT START THE TIMER BEFORE ALL PARAMETERS ARE CONFIGURED!**

In this exercise we will use Timer1 and the CCP1 Module to realize a clock on our display.
Therefore, we need to
1. [finalize configuration of Timer1](#exercise-2a---finalize-timer1s-configuration) so that it can at least count enough input cycles to not overflow within the timespan given by *time_step_in_ms*.
2. [configure the CCP1 module](#exercise-2b---configure-ccp1-module) to trigger an interrupt every *time_step_in_ms* milliseconds, based on Timer1.
3. [update the clock](#exercise-2c---update-the-clock) on our display within the ISR.

If you think you can do this without further instructions - give it a go! It's a great test of your current skills. If not, don't stress yoursevle and just continue reading or click the links in above overview to get directly to relevant instructions.

### Exercise 2.a - Finalize Timer1's configuration
As Timer1 is used as our time base to update the clock, we need to configure it. We already know, that we will use the CPP module. Firstly, because it says so in the instructions and secondly, because you cannot stop *time_step_in_ms* milliseconds exactly with Timer1 without using the CCP1 module.
Thus, we need to finalize the already existing configuration to let Timer1 count long enough to not overflow within *time_step_in_ms* milliseconds. Check the current configuration for the required parameters to calculate the time until overflow.
> [!NOTE]
> The prescaler is already set. To which value? Find out. ;)
> Keep in mind, that a prescaler makes the timer slower, this must be considered in your calculations!

### Exercise 2.b - Configure CCP1 Module
As we know that Timer1 will not overflow within 100ms it's now time to configure the CCP1 module to trigger the interrupt and reset Timer1, like we did in Lab3_Timers.
The configuration is relatively easy, as you only need to
1. calculate the required comparison value after which 100ms are over and set CCPR1 to that value
2. configure CCP1 module's mode so that it resets Timer1 on a comparison match
3. set Timer1 as CCP1's timer


### Exercise 2.c - Update the clock
Last but not least, we will update the clock on our display on every interrupt of CCP1. Therefore, we need to go back to our ISR at the end of main.c and add another if-statement that checks if the interrupt is coming from CCP1. You can use below boilerplate-code for that. Just replace *operandX* with the correct operands to check if CCP1's interrupt is enabled and triggered.
```c
if (operandX && operandX){
    // do we need to reset something here?

    add_ms_to_watch(clock, time_step_in_ms);

    return;
}
```

### Exercise 2.d - Check functionality
Now that our Timer and CCP module are set up and the clock is updatedt on every interrupt, it's time to check if everything works correctly.
Go ahead and press debug to see if the clock is running. Use your mobile's stopwatch to verifiy the correct timing of the clock.
> [!TIP]
> Don't try to stop one second, use 5 to 10 instead as the measurement error introduced by you (yes, you're not a machine and your reaction time is definetly greater than 100ms...) has less impact then.

After checking the timing, have a look at the blinking LED. Does it blink consistently?
If you want to, you can use the simulator and the stopwatch to check the timing of the blinking LED. Just check the timing, we will look for the error in the next exercise.

## Exercise 3
Now that we know that we have a timing issue, we should find the reason for it.  
Remember the lecture about Interrupts and the rule about the execution time of such. *Keep the executioan time of ISRs as short as possible*!
Look at the ISR and find the time consuming part of it. If you found it, proceed to fix it considering below hints. If you did not find it, ask for help before trying to fix it.

> [!NOTE]
> 1. Keep in mind, that an ISR can access the same variables as the main program, if they are globally available. To declare such a global variable you can use below code snippet.
>     ```C
>     static int global_var;
>     int main(void){
>        // ...
>     }
>     ```
> 2. You don't necessarily have to execute an action related to an ISR directly within the ISR. Keep in mind, that the microcontroller executes command very fast and thus ot's completely fine to execute actions that are not time critical (such as updating a display, toggling and LED, or the like) outside the ISR. This could exemplarily be done by checking if a value is larger than a threshold (such as a time difference that should be added to a display) and acting accordingly if this holds true. Thus, we could add a counter for our time differences that need to be added to our clock and simply update the displayed clock with this difference if it is larger than 0.


## Exercise 4 (Experts only, not part of the voting)
As you are now an advanced user of interrupts, it's time to further enhance our little clock. Therefore, we want to improve it from a simple clock to a stopwatch and thus need a start / stop button.
Choose a button of you choice, enable it's dedicated interrupt and implement the required start / stop function.  
Afterwards, try to even reset the stopwatch if it is started again after it was stopped.

> [!WARNING]
> The big button in the middle might seem best for this task, but it is connected to a pin that requires special interrupt configurations. Try to first implement everything with one of the other buttons. If that works and you're still eager to learn, try to use the middle button. Don't hesitate to ask for help, in case of problems. **You are also welcome to ask outside laboratory hours.**
