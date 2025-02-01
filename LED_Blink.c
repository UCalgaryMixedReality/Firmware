/*
 * File:   LED_Blink.c
 * Author: 14036
 *
 * Created on August 18, 2024, 5:33 PM
 */


#include "LED_HEADER.h"



void configurePins(void) {
    INPUT = 1;   // Set RB14 as input
    OUTPUT = 0;  // Set RB13 as output
}

int main(void) {
    configurePins();  // Call the function to configure the pins
    
    while (1) {
        // Example usage
        if (PORTBbits.RB14 == 1) {
            LATBbits.LATB13 = 1;  // Set RB13 high if RB14 is high
        } else {
            LATBbits.LATB13 = 0;  // Set RB13 low if RB14 is low
        }
    }
    
    return 0;
}

