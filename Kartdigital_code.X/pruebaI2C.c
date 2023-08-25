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
uint8_t x;
uint8_t y;
uint8_t z = 2;

char s[3];
char datos;

 char datos[2];
 char datos2[2];
 char mot;

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
        I2C_Master_Write('r');
       I2C_Master_RepeatedStart();
       I2C_Master_Write(0x24);
       I2C_Master_RepeatedStart();
        I2C_Master_Write(0x25);
        x = I2C_Master_Read(0);
        I2C_Master_RepeatedStart();
         I2C_Master_Write(0x24);
        I2C_Master_Write('l');
       I2C_Master_RepeatedStart();
       I2C_Master_Write(0x24);
       I2C_Master_RepeatedStart();
        I2C_Master_Write(0x25);
        y = I2C_Master_Read(0);
         I2C_Master_Stop();
       */
        
        //----------Funcional---------
        I2C_Master_Start();
        I2C_Master_Write(0x24);
        I2C_Master_Write('r');
        I2C_Master_Stop();


       __delay_us(800);
        I2C_Master_Start();
        I2C_Master_Write(0x25);
        x = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_us(800);
       
         I2C_Master_Start();
        I2C_Master_Write(0x24);
        I2C_Master_Write('l');
        I2C_Master_Stop();
        __delay_us(800);
        I2C_Master_Start();
        I2C_Master_Write(0x25);
        y = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_us(800);
        
        
        
        
        datos[0]= (x/100) + '0';
      datos[1]= (x/10) % 10 + '0';
      datos[2]= (x%10) + '0';
      
     datos2[0]= (y/100) + '0';
      datos2[1]= (y/10) % 10 + '0';
     datos2[2]= (y%10) + '0';
         __delay_us(400);
        
     
      
     
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(datos);
    Lcd_Set_Cursor(2,6);
    Lcd_Write_String(datos2);

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