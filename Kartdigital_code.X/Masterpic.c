/*
 * File:   Masterpic.c
 * Author: Diego
 *
 * Created on 11 de agosto de 2023, 06:35 PM
 */

#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <stdint.h>}
#include <stdio.h>         // for sprintf
#include "USARTmodl.h"
#define _XTAL_FREQ 8000000
#include <string.h>
char datos[20];
uint8_t RPMR, RPML,aceleracX;
/********************** end UART functions **************************/
void __interrupt() isr (void)
{
    NOP();
}
void setup(void);


 char uart_read(){
 if(PIR1bits.RCIF== 0){
     if (RCSTAbits.OERR){
         RCSTAbits.CREN =0;
         NOP();
         RCSTAbits.CREN =1;
 }
     return RCREG;
 }
 else
     return 0;
 }
// main function
void main(void)
{
    setup();
  OSCCON = 0x70;    // set internal oscillator to 8MHz
 
  UART_Init(9600);  // initialize UART module with 9600 baud
 
  __delay_ms(2000);  // wait 2 seconds
 
 
 
 
 char text[9];
  while(1)
  {
      RPMR = 100;
      RPML= 50;
      aceleracX= 24;
       
      
       
      
          
      datos[0]= RPMR;
      datos[4]= RPML;
      datos[8]= aceleracX;
      UART_Print ("\r\n");
           // sprintf(text, "%03u\r\n", RPMR);
             UART_Print(datos);
             __delay_ms(700);
      //Enviar datos al terminal
    if ( UART_Data_Ready() )  // if a character available
    {
      uint8_t c = UART_GetC();  // read from UART and store in 'c'
      UART_PutC(c);  // send 'c' via UART (return the received character back)
    }
             
             
             
             
             
             
  }
 
}
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    
    TRISD= 0;
    PORTD = 0;
 

              
 
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
}
