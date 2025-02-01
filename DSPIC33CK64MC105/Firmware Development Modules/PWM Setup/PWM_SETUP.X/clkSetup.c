/*
 * File:   CLK_Setup.c
 * Author: 14036
 *
 * Created on November 17, 2024, 10:18 AM
 * This file initiates a 20 MHz clock using the phase lock loop methodology
 * More information on this setup can be found in section 9 of the datasheet 
 */


#include "xc.h"

void clk_setup(void)
{
    /*
     FPLLO = FPLLI   M /(N1 *N2*?N3), where FPLLI is the default 8MHz FRC CLK
    M = PLLFBDIV[7:0] 
    N1 = PLLPRE[3:0]
    N2 = POST1DIV[2:0]
    N3 = POST2DIV[2:0]
    
     We want FCY to be 20 MHz, which Means FOSC needs to be 40 MHz, which means FPLLO needs to be 80 MHz
     
     */
    #pragma config PLLKEN = ON    // PLL Lock Enable (PLL clock output will be disabled if LOCK is lost)
    // FOSCSEL
    #pragma config FNOSC = FRCPLL // Select PLL output CLK
    PLLFBDbits.PLLFBDIV = 50;     // Set M to 50
    CLKDIVbits.PLLPRE = 1;        // N1=1
    PLLDIVbits.POST1DIV = 5;      // Set N2 equal to 5
    PLLDIVbits.POST2DIV = 1;      // N3=1
    I2C1BRG = 24;                 // Set Baud rate based on FCY = 20 MHz and FSCL = 400 kHz
    
}

void __attribute__((__interrupt__, no_auto_psv))_T1Interrupt(void)
{
    IFS0bits.T1IF   =   0;  // Clear the timer interrupt flag
    LATBbits.LATB0 = !LATBbits.LATB0; // Toggle output on RB0 
}



void timer_setup(void)
{
//    T1CONbits.TCKPS = 0b11; // Set pre-scaler to 1:256, frequency to 78.125 kHz
    PR1 = 20000;            // Set period to achieve ~1 ms interrupt (adjust as needed)
    TMR1 = 0;               // Clear the timer count register
    IFS0bits.T1IF = 0;      // Clear Timer1 interrupt flag to ensure no pending interrupts are left from previous operations.
                            // This runs only once during initialization, preventing an immediate interrupt after enabling the timer.
                            // Subsequent interrupts will be handled and cleared within the interrupt service routine (ISR).

    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    T1CONbits.TON = 1;      // Start Timer1
    T1CONbits.TCS = 1;      // External Clock source selected by TECS[1:0]
    T1CONbits.TECS = 2;     // Choosing FOSC CLK for Timer1 Extended Clock Select bits (should be 20 MHz)



    }
int mainClk(void) {
    
    clk_setup();                           // Initialize the clock speed
    TRISBbits.TRISB0 = 0;                  // Set RB0 as output (for toggling)
    timer_setup();                         // Initialize the timer
    __builtin_enable_interrupts();         // Enable global interrupts

    while(1) {
        // Main loop can run other code
        
    }
    
    
    
    return 0;
}
