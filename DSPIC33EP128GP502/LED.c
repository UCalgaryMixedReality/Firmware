/*
 * File:   LED.c
 * Author: 14036
 *
 * Created on November 25, 2024, 7:14 PM
 */


#include "xc.h"

int main(void) {
    TRISAbits.TRISA1 = 0; //Designate this pin as an output 
    LATAbits.LATA1 = 1;  // Turn on LED connected to RA1
    return 0;
}
