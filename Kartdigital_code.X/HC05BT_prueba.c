/*
 * File:   HC05BT_prueba.c
 * Author: diego
 *
 * Created on 13 de agosto de 2023, 12:39 AM
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
char valpot;
uint16_t map(uint16_t varmap,uint16_t minval,uint16_t maxval, uint16_t minsal, uint16_t maxsal);
uint16_t a =0;
/********************** end UART functions **************************/
void __interrupt() isr (void)
{
    if(PIR1bits.ADIF){
        //Interrupci�n
         if (ADCON0bits.CHS ==0)
          valpot = ADRESH;
         PIR1bits.ADIF =0;
        
     
        
    }
    
}
void setup(void);
void preguntas (void);

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
 
  UART_Print("1.Leer potenciometro\r\n");  // UART print
 
  __delay_ms(1000);  // wait 1 second
 
  UART_Print(message);  // UART print message
 
  __delay_ms(1000);  // wait 1 second
 
  UART_Print("\r\n");  // start new line
 ADCON0bits.GO =1;
 char text[9];
  while(1)
  {
       if (ADCON0bits.GO ==0)
     ADCON0bits.GO =1;
      //Recibir datos del terminal
     /*  char datos = uart_read();
      if (datos == '5'){
          UART_Print("Numero 5\r\n");
          RCREG =0;
          TRISDbits.TRISD0 =1;
          TRISDbits.TRISD1 =0;
          TRISDbits.TRISD2 =1;
          TRISDbits.TRISD3 =0;
      }
      * */
      //Probando usando un break
       
       a ++;
       PORTD = a;
       __delay_ms(700);
       UART_Print ("\r\n");
            sprintf(text, "%03u\r\n", a);
             UART_Print(text);
             __delay_ms(700);
      switch (uart_read()){
          case '1': 
              
               
      
               valpot = ADRESH;
              uint16_t varvolt2 = map(valpot,0,255,0,5);
     
              UART_Print ("\r\n");
            sprintf(text, "%03u\r\n", varvolt2);
            UART_Print(text);
   
  
              preguntas();
             RCREG ='0';
             
             break;
           case '+': 
               __delay_us(2000);
               PORTD ++;
               RCREG ='0';
               
               break;
                 case '-': 
               __delay_us(2000);
               PORTD --;
               RCREG ='0';
               
               break;
          
      }
      //Enviar datos al terminal
    if ( UART_Data_Ready() )  // if a character available
    {
      uint8_t c = UART_GetC();  // read from UART and store in 'c'
      UART_PutC(c);  // send 'c' via UART (return the received character back)
    }
 
  }
 
}
void setup(void){
    ANSEL = 0b00000011;
    ANSELH = 0;
    
    TRISA = 0xFF;
    TRISD= 0;
    PORTD = 0;
 
    // Configuraci�n del ADC
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b01; //FOSC/32
    ADCON0bits.CHS = 0;
    ADCON0bits.ADON= 1;
    __delay_us(50);
              //Configuraci�n de las interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
}
void preguntas(void)
{
    UART_Print ("1.Leer potenciometro\r\n");
    UART_Print (message);
}
uint16_t  map(uint16_t varmap,uint16_t minval,uint16_t maxval, uint16_t minsal, uint16_t maxsal){
  float  valmap =((varmap - minval) * (maxsal - minsal)) / (maxval - minval) + minsal;
  return valmap;
}