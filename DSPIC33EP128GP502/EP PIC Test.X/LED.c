/*
 * File:   LED.c
 * Author: 14036
 *
 * Created on November 25, 2024, 7:14 PM
 */
#include <xc.h>
#include <libpic30.h>               // Required for __delay32()


#define FCY 4000000UL               // Define instruction clock frequency (Fosc/2)

void delay_half_second(void) {
    __delay32(4000000UL);           // 0.5-second delay (FCY * 0.5)
}

int main(void) {
    TRISAbits.TRISA1 = 0;           // Configure RA1 as output
    LATAbits.LATA1 = 0;             // Initialize LED state to OFF

    while (1) {
        LATAbits.LATA1 = 1;         // Turn ON LED
//        delay_half_second();        // Wait 0.5 second
//        LATAbits.LATA1 = 0;         // Turn OFF LED
//        delay_half_second();        // Wait 0.5 second
    }

    return 0;
}