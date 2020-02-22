/*
 * File:   Master_main.c
 * Author: josue
 *
 * Created on February 19, 2020, 2:17 PM
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
#include "LCD_8bits.h"
#include "I2C.h"

#define _XTAL_FREQ 4000000
uint8_t valorADC = 0;
uint16_t * voltaje_map;

uint16_t * mapear(uint8_t valor, uint8_t limReal, uint8_t limSup);

void main(void) {
    TRISD = 0;
    TRISC0 = 0;
    TRISC1 = 0;
    LCD_init();
    LCD_clear();
    LCD_Set_Cursor(1,1);
    LCD_Write_String("S1:");
    I2C_Master_Init(100000); //inicializa I2C a 100kHz
    
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        valorADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(100);
        
        voltaje_map = mapear(valorADC, 255, 5);
        LCD_Set_Cursor(2,0);
        LCD_Write_Character(uint_to_char(voltaje_map[0]));
        LCD_Write_Character('.');
        LCD_Write_Character(uint_to_char(voltaje_map[1]));
        LCD_Write_Character(uint_to_char(voltaje_map[2]));
        LCD_Write_Character('V');
    }
    return;
}

uint16_t * mapear(uint8_t valor, uint8_t limReal, uint8_t limSup){
    uint16_t resultado[3] = {0,0,0};  // u.d1.d2  [u, d1, d2]
    uint16_t dividendo = valor*limSup;
    while (limReal <= dividendo){
        resultado[0] = resultado[0] + 1;
        dividendo = dividendo - limReal;
    }
    dividendo = dividendo *10;
    while (limReal <= dividendo){
        resultado[1] = resultado[1] +1;
        dividendo = dividendo - limReal;
    }
    dividendo = dividendo *10;
    while (limReal <= dividendo){
        resultado[2] = resultado[2] +1;
        dividendo = dividendo - limReal;
    }
    
    return resultado;
}