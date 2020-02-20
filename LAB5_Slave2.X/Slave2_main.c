/*
 * File:   Slave2_main.c
 * Author: josue
 *
 * Created on February 19, 2020, 2:20 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000

uint8_t banderaBoton = 0;
uint8_t banderaDO = 0;
uint8_t banderaUP = 0;

void config_PUERTOS(void);
void press_Subir(void);
void press_Bajar(void);

void __interrupt() ISR(void){
    if (INTCONbits.RBIF == 1 && INTCONbits.RBIE == 1){   //atencion IOCB
        INTCONbits.RBIF = 0;
        if (banderaBoton == 0){
            banderaBoton = 1;
            INTCONbits.RBIE = 0;
        }
    }
      return;  
    }

void main(void) {
    config_PUERTOS();
    INTCONbits.GIE = 1;
    while(1){
        press_Subir();
        press_Bajar();
    }
    return;
}

void config_PUERTOS(void){
    //PUERTOS
    TRISA = 0;
    TRISB = 0b00110000;
    PORTA = 0; 
    ANSEL = 0;
    ANSELH = 0; 
    WPUB = 0b00110000;
    OPTION_REGbits.nRBPU = 0;
    
    //Interrupciones
    IOCB = 0b00110000;;  //RB0 y RB2 tiene interrupcion
    INTCONbits.RBIE = 1;
    return;
}

void press_Subir(void){
    if (banderaBoton == 1){
        if (banderaUP == 0){
            if (PORTBbits.RB5 == 0){
                __delay_ms(69);
                PORTA = PORTA + 1;
                if (PORTA >= 15){
                    PORTA = 15;
                }
                banderaBoton = 0;
                banderaUP = 1;
                INTCONbits.RBIE = 1;
            }  
        }  
    }
    if (banderaUP == 1){
        if (PORTBbits.RB5 == 1){
        __delay_ms(69);
        banderaUP = 0;
        }
    }    
}

void press_Bajar(void){
    if (banderaBoton == 1){
        if (banderaDO == 0){
            if (PORTBbits.RB4 == 0){
                __delay_ms(69);
                PORTA = PORTA - 1;
                if (PORTA == 0 || PORTA > 200){
                    PORTA = 0;
                }
                banderaBoton = 0;
                banderaDO = 1;
                INTCONbits.RBIE = 1;
            }  
        }  
    }
    if (banderaDO == 1){
        if (PORTBbits.RB4 == 1){
        __delay_ms(69);
        banderaDO = 0;
        }
    }    
}