/*
 * File:   main.c
 * Author: 14036
 *
 * Created on November 16, 2024, 10:44 PM
 */


#include "xc.h"


void pwmSetup(void) {

    /* 4 key registers need to be configured for basic PWM functionality 
     These include:
        PGCONL
        PGCONH
        PGIOCONL
        PGIOCONH*/
    
    //PGCONL Initialization 
    PG1CONLbits.ON = 0; 
    PG1CONLbits.TRGCNT = 0X0; // PWM generator produces 1 clk cycle after triggered 
    PG1CONLbits.CLKSEL = 0x0;
    PG1CONLbits.MODSEL = 0x4; //Selects Center-Aligned PWM mode
    
    
    //PG1CONH setup
    PG1CONHbits.MDCSEL = 0; //PWM uses PG1DC register for duty cycle
    PG1CONHbits.MPERSEL = 0; // PWM uses PG1PER register for period
    PG1CONHbits.MPHSEL = 0;
    PG1CONHbits.MSTEN = 0;
    PG1CONHbits.UPDMOD = 0x0;
    PG1CONHbits.TRGMOD = 0;
    PG1CONHbits.SOCS = 0x0;
    
    //PG1IOCONL setup
    PG1IOCONLbits.CLMOD = 0;
    PG1IOCONLbits.SWAP = 0; //map PWML and PWMH to respective pins
    PG1IOCONLbits.OVRENH = 0; //PWM provides data for PWMH pin
    PG1IOCONLbits.OVRENL = 1; //OVERDAT[0] provides data for PWML pin
    PG1IOCONLbits.OVRDAT = 0; // disables PWML
    PG1IOCONLbits.OSYNC = 0;
    PG1IOCONLbits.FLTDAT = 0;
    PG1IOCONLbits.FFDAT = 0;
    PG1IOCONLbits.DBDAT = 0;
    
    
    //PG1IOCONH setup
    PG1IOCONHbits.CAPSRC = 0;
    PG1IOCONHbits.DTCMPSEL = 0; //ONLY PWMH being used so no deadtime needed
    PG1IOCONHbits.PMOD = 1; //PWM operates in independent mode
    PG1IOCONHbits.PENH = 1; //PWM controls PWM1H pin
    PG1IOCONHbits.PENL = 0; //PWM does not control PWM1L pin
    PG1IOCONHbits.POLH = 0; //PWM1H is active high (zero phase shift)
    PG1IOCONHbits.POLL = 0; //Pin is disabled so doesn't matter
    
    //PG1 duty cycle and period register initializations
    
    PG1PER = 1000;   // Set period for the PWM
    PG1DC = 500;     // Set 50% duty cycle
    PG1LEBL = 0; // The minimum blanking period is eight clock periods, which occurs when LEB[15:3] = 0.
    
    PG1CONLbits.ON = 1; //enable module
            
    
    
    
    

   
}


int main(void) {
    
    TRISAbits.TRISA1 = 0; //test output
    TRISBbits.TRISB14 = 0; // Set RB14 (PWM1H) as output
    mainClk();   // runs the clock setup
    pwmSetup();  //run the PWM initialization function
    
    
    while(1) {
    //run main code in here
        LATAbits.LATA1 = 1; //keep test output high
    }
    return 0;
}

