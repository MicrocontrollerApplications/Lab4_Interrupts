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

## Relevant registers
For today's laboratory we will need the previously used registers to configure our I/O pins and the registers 
introduced in the lecture. All those registers are listed below. Please check them to ensure you are aware of all
relevant registers for today.
> [!NOTE]
> <details>
> <summary>### I/O</summary>
> <strong>TRISB</strong> sets the direction of a pin: "0" = output, "1" = input<br>
> **PORTB** reads the logic level at the pin: "0" = 0-0.8 V, "1" = Vdd (1.6-3.3 V or 2.0-5.0 V for TTL inputs)<br>
> **LATB** set the output voltage of a pin: "0" = 0 V, "1" = Vdd (3.3 V or 5 V)<br>
> **ANSELB** sets a pin as digital or analog: "0" = digital, "1" = analog<br>
> **OSCCON** sets the oscillator freq.: 0x50 = 4 MHz, 0x30 = 1 MHz. The instruction freq. are 1 MHz and 250 kHz respectively<br>
> **\<Register\>bits.X** TRISBbits.TRISB3 or LATBbits.LATB3 or PORTBbits.RB3 is the direct access to the bit 3 via a struct (see structures in C).
> </details>
> ### Timer X
> **TxCON** to configure Timerx (today x will be 0 and 1)<br>
> **TMRx\<H/L\>** to set the timer to a given (initial) value<br>
> **INTCON** to check Timer0's interrupt (i.e. overflow) flag<br>
> ### Capture Compare Module
> **CCPxCON** configure the Capture Compare Module<br>
> **CCPx\<H/L\>** set comparison value for comparison mode of Capture Compare Module<br>
> **CCPTMRS0** set timer to be compared with CCPx<H/L><br>
> **PIR1** to check Capture Compare Module's interrupt flag<br>
> ### Interrupts
> 
