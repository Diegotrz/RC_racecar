/*
 * File:   pruebaI2C.c
 * Author: diego
 *
 * Created on 21 de agosto de 2023, 04:24 PM
 */

// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include "stdio.h"
#include <pic16f887.h>
#include "I2C.h"
#include "LCD.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
uint8_t dato;
int x = 100;
uint8_t y = 94;
uint8_t z = 2;
static char datos[] = "varl: 000:000:000";
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();
    I2C_Master_Init(100000);
    
    
    while(1){
        /*
        I2C_Master_Start();
        I2C_Master_Write(0x24);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(200);
       */
       // I2C_Master_Start();
       // I2C_Master_Write(0x25);
       // datos = I2C_Master_Read(0);
       // I2C_Master_Stop();
        __delay_ms(200);
        /*
        if (dato == 1){
            PORTD = 255;
        }
       if (dato == 0){
            PORTD = 0;
        }
         * */
        int x = 111;
         datos[6]  = x   / 10 + '0';
        datos[7]  = x   % 10 + '0';
        datos[8] = x   / 10 + '0';
        Lcd_Set_Cursor(1,1);
    
    Lcd_Write_String(datos);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("Hola");
    }
  
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}