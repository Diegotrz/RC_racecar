/*
 * File:   pruebaADXL345.c
 * Author: Diego
 *
 * Created on 19 de agosto de 2023, 06:56 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "LCD.h"
#include "stdio.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t  i, x,y,z,Xval,Xval2,Yval,Yval2,Zval,Zval2;
char s[20];
char s2[20];
char s3[20];
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void I2C_Init(uint32_t i2c_clk_freq)

{
  SSPCON  = 0x28;  // configure MSSP module to work in I2C mode
  SSPADD  = (_XTAL_FREQ/(4 * i2c_clk_freq)) - 1;  // set I2C clock frequency
  SSPSTAT = 0;
}



//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();
    I2C_Init(100000);
     I2C_Master_Start();
        I2C_Master_Write(0x2D);
        I2C_Master_Write(0b00001000);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x31);
        I2C_Master_Write(0b00000010);  
        I2C_Master_Stop();
        
    while(1){
        
  
       
        I2C_Master_Start();
        I2C_Master_Write(0x32);
        Xval = I2C_Master_Read(0);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x33);
        Xval2 = I2C_Master_Read(0);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x34);
        Yval = I2C_Master_Read(0);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x35);
        Yval2 = I2C_Master_Read(0);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x36);
        Zval = I2C_Master_Read(0);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x37);
        Zval2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        __delay_ms(200);
         Lcd_Clear();
         x= (Xval2<<8)| (Xval & 0xFF);
         y= Yval|Yval2 <<8;
         z= Zval|Zval2 <<8;
        sprintf(s, "%u", x);
        sprintf(s2, "%u", y);
        sprintf(s3, "%u", z);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(s);
    Lcd_Set_Cursor(1,5);
    Lcd_Write_String(s2);
    Lcd_Set_Cursor(1,12);
    Lcd_Write_String(s3);
   
  
    }
    
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    TRISC6 = 0;
    TRISC7 = 0;
    TRISA =0;
    TRISB = 0;
    TRISD = 0;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC1 = 1;  
    PORTCbits.RC2 = 1;
 
// Configuración del oscilador
    OSCCONbits.IRCF =   0b0111; //8MHz
    OSCCONbits.SCS = 1;
    
   
}