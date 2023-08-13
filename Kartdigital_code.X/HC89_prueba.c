/*
 * File:   Prelab 1
 * Author: Diego Terraza
 *
 * Created on 13 de julio de 2023, 11:10 AM
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

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
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "LCD.h"
#include "stdio.h"

/*
 *Variables
 */
uint8_t a= 0;
int b= 0;
int rpm= 0;
/*
 * Prototipos de funciones
 */
void setup(void); //Prototipo de la función que contiene todo el setup
/*
 
 *Interrupción
 */
void __interrupt() isr (void)
{
    if (INTCONbits.RBIF ){

        
        NOP();
    }
    if (INTCONbits.T0IF){
        //b==31 para un segundo
        // b == 305 para 10 segundos
        if (b<=305){
           b ++;           
        }
        
        if (b==305){
            rpm = a/2;
            PORTA = rpm;
            a= 0;
            b= 0;
            
        }
        INTCONbits.T0IF = 0;
        TMR0 = 0;
    
    }
}
/*
 *---------------Main-------------
 */
void main (void)
{
    
    setup(); //Mandamos a llamar la función del setup
    Lcd_Init();
    while(1)
    {
        if (!PORTBbits.RB0){ //Verifica si la interrupción del puerto RB0 ha cambiado
            while (!RB0);
                a ++;  
               
        }
    if (!PORTBbits.RB1){
            while (!RB1) ;
                a --; 
                
                         
        }
        
        char s[20];
        
        Lcd_Set_Cursor(1,1);
        sprintf(s, "%u", rpm);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(s);
        
    }
    
    
    
}
/*
 * Funciones
 */
void setup(void){
    ANSEL = 0b00000011;
    ANSELH = 0;
    TRISA = 0;
   
    TRISB = 0b11111111;
    TRISD= 0;
    OPTION_REGbits.nRBPU =  0;
    WPUB = 0b11111111;
    TRISC6 = 0;
    TRISC7 = 0;
    PORTA = 0;
    PORTD = 0;
    PORTC = 0;
    // Configuración del oscilador
    OSCCONbits.IRCF =   0b0111; //8MHz
    OSCCONbits.SCS = 1;
    //Timer0 Registers Prescaler= 256 - TMR0 Preset = 0 - Freq = 30.52 Hz - Period = 0.032768 seconds
OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
OPTION_REGbits.T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
OPTION_REGbits.PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
OPTION_REGbits.PS2 = 1;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
OPTION_REGbits.PS1 = 1;
OPTION_REGbits.PS0 = 1;
TMR0 = 0;             // preset for timer register

    
    
    //Configuración de las interrupciones
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE  = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
}