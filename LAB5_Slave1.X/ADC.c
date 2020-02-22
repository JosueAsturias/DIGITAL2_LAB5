/*
 * File:   ADC.c
 * Author: josue
 *
 * Created on January 30, 2020, 4:06 PM
 */

#include <xc.h>
#include <stdint.h>
#include "ADC.h"

#define _XTAL_FREQ 4000000


void ADConfig(uint8_t oscFreq,uint8_t canal, unsigned char justificado){
    switch(oscFreq){
        case 1:
            ADCON0bits.ADCS = 0b00;  
            break;
        case 4:
            ADCON0bits.ADCS = 0b01;
            break;
        case 8:
            ADCON0bits.ADCS = 0b10;
            break;
        case 20:
            ADCON0bits.ADCS = 0b11;
            break;
        default:
            ADCON0bits.ADCS = 0b01;
    }
    switch(justificado){
        case 'H':
            ADCON1bits.ADFM = 0;
            break;
        case 'L':
            ADCON1bits.ADFM = 1;
            break;
        default:
            ADCON1bits.ADFM = 0;
    }
    switch (canal){
        case 0:
            TRISAbits.TRISA0 = 1;
            ANSELbits.ANS0 = 1;
            ADCON0bits.CHS = 0;
            break;
        case 1:
            TRISAbits.TRISA1 = 1;
            ANSELbits.ANS1 = 1;
            ADCON0bits.CHS = 1;
            break;    
        case 2:
            TRISAbits.TRISA2 = 1;
            ANSELbits.ANS2 = 1;
            ADCON0bits.CHS = 2;
            break;    
        case 3:
            TRISAbits.TRISA3 = 1;
            ANSELbits.ANS3 = 1;
            ADCON0bits.CHS = 3;
            break;
        case 4:
            TRISAbits.TRISA5 = 1;
            ANSELbits.ANS4 = 1;
            ADCON0bits.CHS = 3;
            break;
        case 5:
            TRISEbits.TRISE0 = 1;
            ANSELbits.ANS5 = 1;
            ADCON0bits.CHS = 5;
            break;
        case 6:
            TRISEbits.TRISE1 = 1;
            ANSELbits.ANS6 = 1;
            ADCON0bits.CHS = 6;
            break;
        case 7:
            TRISEbits.TRISE2 = 1;
            ANSELbits.ANS7 = 1;
            ADCON0bits.CHS = 7;
            break;
        // seguir con ANSELH    
            
            
    }
    ADCON0bits.ADON = 1;
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    __delay_ms(30);
    ADCON0bits.GO_nDONE = 1;
}

uint8_t AnalogRead_8(unsigned char just){
    uint8_t conversion = 0;
    switch (just){
        case 'H':
            conversion = ADRESH;
            break;
        case 'L':
            conversion = ADRESL;
            break;
        default:
            conversion = ADRESH;
    }
    return (conversion);
}

void ADCinit(){
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.GIE = 1;
    __delay_ms(20);
    ADCON0bits.GO_nDONE = 1;
    return;
}

void ADC_CHselect(uint8_t canal){
    switch (canal){
        case 0:
            TRISAbits.TRISA0 = 1;
            ANSELbits.ANS0 = 1;
            ADCON0bits.CHS = 0;
            break;
        case 1:
            TRISAbits.TRISA1 = 1;
            ANSELbits.ANS1 = 1;
            ADCON0bits.CHS = 1;
            break;    
        case 2:
            TRISAbits.TRISA2 = 1;
            ANSELbits.ANS2 = 1;
            ADCON0bits.CHS = 2;
            break;    
        case 3:
            TRISAbits.TRISA3 = 1;
            ANSELbits.ANS3 = 1;
            ADCON0bits.CHS = 3;
            break;
        case 4:
            TRISAbits.TRISA5 = 1;
            ANSELbits.ANS4 = 1;
            ADCON0bits.CHS = 3;
            break;
        case 5:
            TRISEbits.TRISE0 = 1;
            ANSELbits.ANS5 = 1;
            ADCON0bits.CHS = 5;
            break;
        case 6:
            TRISEbits.TRISE1 = 1;
            ANSELbits.ANS6 = 1;
            ADCON0bits.CHS = 6;
            break;
        case 7:
            TRISEbits.TRISE2 = 1;
            ANSELbits.ANS7 = 1;
            ADCON0bits.CHS = 7;
            break;
        // seguir con ANSELH    
            
            
    }
}