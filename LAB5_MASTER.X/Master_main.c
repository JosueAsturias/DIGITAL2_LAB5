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
#include "RTC.h"

#define _XTAL_FREQ 4000000
int seg = 0;
int min = 0;
int hora = 0;
int dia = 1;
int datum = 23;
int mes = 2;
int jahr = 20;

uint8_t valorADC = 0;
uint8_t contador = 0;
uint16_t * voltaje_map;
uint16_t * temp_array;

uint16_t * mapear(uint8_t valor, uint8_t limReal, uint8_t limSup);
void display_Uhrzeit(uint8_t fila, uint8_t columna);


void main(void) {
    TRISD = 0;
    TRISC0 = 0;
    TRISC1 = 0;
    LCD_init();
    LCD_clear();
    LCD_Set_Cursor(1,1);
    LCD_Write_String("S1:  S2:   S3:");
    LCD_Set_Cursor(2,6);
    LCD_Write_String("0x");
    I2C_Master_Init(100000); //inicializa I2C a 100kHz
//    Zeit_Datum_Set();
//    __delay_ms(20);

    
    
    while(1){
        I2C_Master_Start();            // comunicacion con Slave1
        I2C_Master_Write(0x61);
        valorADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();            // comunicacion con Slave2
        I2C_Master_Write(0x71);
        contador = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        uint8_t temperatura = get_Temp();  //comunicacion con RTC y obt. temp.
        uint8_t signo = temperatura & 0b10000000; //ver que signo tiene la conv.
        LCD_Set_Cursor(2,11);
        if (signo){ LCD_Write_Character('-'); }
        else{ LCD_Write_Character(' '); }
        
        temp_array = uint_to_array(temperatura & 0b01111111); 
        LCD_Write_Character(uint_to_char(temp_array[1]));
        LCD_Write_Character(uint_to_char(temp_array[2]));
        LCD_Write_Character(223);
        LCD_Write_Character('C');
        
        
        
        // *********** mostrar en LCD valores de sensores
        voltaje_map = mapear(valorADC, 255, 5);               // sensor 1
        LCD_Set_Cursor(2,0);
        LCD_Write_Character(uint_to_char(voltaje_map[0]));
        LCD_Write_Character('.');
        LCD_Write_Character(uint_to_char(voltaje_map[1]));
        LCD_Write_Character(uint_to_char(voltaje_map[2]));
        LCD_Write_Character('V');
        
        LCD_Set_Cursor(2,8);                                  // sensor 2
        LCD_Write_Character(uint_to_char(contador));
        
        
    }
    return;
}


/********Funcion que mapea un valor en una escala a un valor en otra escala
         y luego guarda los digitos en un array                             */
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
    
    return(resultado);
}

/*Funcion que muestra hora:minutos:segundos obtenido del RTC*/
void display_Uhrzeit(uint8_t fila, uint8_t columna){
    char seg_u = seg%10;
    char seg_d = seg/10;
    char min_u = min%10;
    char min_d = min/10;
    char Uhr_u = hora%10;
    char Uhr_d = hora/10;
    
    LCD_Set_Cursor(fila, columna);
    LCD_Write_Character(seg_d + '0');
    LCD_Write_Character(seg_u + '0');
    LCD_Write_Character(':');
    LCD_Write_Character(min_d + '0');
    LCD_Write_Character(min_u + '0');
    LCD_Write_Character(min_d + '0');
    LCD_Write_Character(min_u + '0');
}