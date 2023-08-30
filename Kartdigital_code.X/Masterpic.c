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
#include <stdint.h>
#include "I2C.h"
#include <stdio.h>         // for sprintf
#include "USARTmodl.h"
#define _XTAL_FREQ 8000000
#include <string.h>
#include "LCD.h"
char datos[20];
uint8_t RPMR, RPML,aceleracX;
uint8_t  i, second, minute, hour, m_day, month, year;
/********************** end UART functions **************************/
void __interrupt() isr (void)
{
    NOP();
}
void setup(void);
uint8_t bcd_to_decimal(uint8_t number) {
  return((number >> 4) * 10 + (number & 0x0F));
}
 
// convert decimal to BCD function
uint8_t decimal_to_bcd(uint8_t number) {
  return(((number / 10) << 4) + (number % 10));
}   
void RTC_display()
{
  static char Time[] = "TIME: 00:00:00";
  static char Date[] = "DATE: 00/00/2000";
 
  // convert data from BCD format to decimal format
  second = bcd_to_decimal(second);
  minute = bcd_to_decimal(minute);
  hour   = bcd_to_decimal(hour);
  m_day  = bcd_to_decimal(m_day);
  month  = bcd_to_decimal(month);
  year   = bcd_to_decimal(year);
  // end conversion
 
  // update time
  Time[6]  = hour   / 10 + '0';
  Time[7]  = hour   % 10 + '0';
  Time[9]  = minute / 10 + '0';
  Time[10] = minute % 10 + '0';
  Time[12] = second / 10 + '0';
  Time[13] = second % 10 + '0';
  // update date
  Date[6]  = m_day  / 10 + '0';
  Date[7]  = m_day  % 10 + '0';
  Date[9]  = month  / 10 + '0';
  Date[10] = month  % 10 + '0';
  Date[14] = year   / 10 + '0';
  Date[15] = year   % 10 + '0';
  //Lcd_Clear();
 Lcd_Set_Cursor(1,1);
  
  Lcd_Write_String(Time);
  
}

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
  minute = decimal_to_bcd(0);
    second = decimal_to_bcd(0);
    I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x01);    
        I2C_Master_Write(minute);   
        
        I2C_Master_Stop();
        __delay_ms(200);
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);    
        I2C_Master_Write(second);   
        
        I2C_Master_Stop();
 
 
 
 char text[9];
  while(1)
  {
      /*
       I2C_Master_Start();
        I2C_Master_Write(0x24);
        I2C_Master_Write('r');
        I2C_Master_Stop();
       __delay_us(800);
        I2C_Master_Start();
        I2C_Master_Write(0x25);
        RPMR = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_us(800);
      */
        
        //Código del RTC
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        second = I2C_Master_Read(0);
        I2C_Master_Write(0x01);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        minute = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
       RTC_display();
      //uint8_t fm = (RPMR/1000);
      //datos[0]= fm + '0';
    //  datos[0]= (RPMR/100)%10 + '0';
     // datos[1]= (RPMR/10)%10 + '0';
      
      //Funcional solo con centenas
      datos[0]= (RPMR/100) + '0';
      datos[1]= (RPMR/10) % 10 + '0';
      datos[2]= (RPMR%10) + '0';
    
     // datos[4]= RPML;
    //  datos[8]= aceleracX;
      
      UART_Print ("\r\n");
           // sprintf(text, "%03u\r\n", RPMR);
             UART_Print(datos);
             UART_Print ("\r\n");
            
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
    // Configuración del oscilador
    OSCCONbits.IRCF =   0b0111; //8MHz
    OSCCONbits.SCS = 1;
    
}
