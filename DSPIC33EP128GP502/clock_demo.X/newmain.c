#include <xc.h>
#include <stdint.h>

// === Configuration Bits ===
#pragma config FNOSC = FRCPLL      // Fast RC Oscillator with PLL
#pragma config FCKSM = CSECMD      // Clock switching enabled, Fail-Safe disabled
#pragma config IESO = OFF          // Internal External Switch Over Mode disabled
#pragma config POSCMD = NONE       // Primary Oscillator disabled
#pragma config OSCIOFNC = ON       // OSCO/CLKO pin as digital I/O

#define FCY 20000000UL             // Instruction clock
#include <libpic30.h>

// === Function Prototypes ===
void init_oscillator(void);
void init_timer1(void);
void init_io(void);

// === Main Program ===
int main(void) {
    init_oscillator();
    init_io();
    init_timer1();

    while (1) {
        // Idle loop, LED toggle handled in ISR
    }
    return 0;
}

// === Oscillator Configuration ===
void init_oscillator(void) {
    CLKDIVbits.PLLPRE = 0;     // N1 = 2
    PLLFBD = 41;               // M = 43
    CLKDIVbits.PLLPOST = 1;    // N2 = 4
    while (OSCCONbits.LOCK != 1); // Wait for PLL to lock
}

// === IO Configuration ===
void init_io(void) {
    TRISBbits.TRISB15 = 0;    // Set RB15 as output
    LATBbits.LATB15 = 0;      // Start LED off
}

// === Timer1 Configuration ===
void init_timer1(void) {
    T1CONbits.TCKPS = 0b11;   // Prescaler 1:256
    PR1 = 78125;              // 1 Hz toggle rate
    TMR1 = 0;
    IFS0bits.T1IF = 0;        // Clear interrupt flag
    IEC0bits.T1IE = 1;        // Enable Timer1 interrupt
    T1CONbits.TON = 1;        // Start Timer1
}

// === Timer1 ISR ===
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    LATBbits.LATB15 ^= 1;     // Toggle RB15 (LED)
    IFS0bits.T1IF = 0;        // Clear interrupt flag
}