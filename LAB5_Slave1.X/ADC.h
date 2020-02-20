/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __ADC_H
#define	__ADC_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

void ADConfig(uint8_t oscFreq,uint8_t canal, unsigned char justificado);

uint8_t AnalogRead_8(unsigned char just);

void ADCinit();

void ADC_CHselect(uint8_t canal);


#endif	/* XC_HEADER_TEMPLATE_H */