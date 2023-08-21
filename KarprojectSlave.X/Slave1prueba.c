/*
 * File:   Slave1prueba.c
 * Author: diego
 *
 * Created on 21 de agosto de 2023, 03:31 PM
 */
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
#include "PWM.h"
#include <pic16f887.h>
#include "stdio.h"
#include "LCD.h"
#define _XTAL_FREQ 8000000
/*
 *Constantes
 */
int i;
float val;
/*
 *Variables
 */
unsigned int valadr,valpot0,valpot1,valprom;
uint8_t a= 0;
uint8_t b= 0;
int c= 0;
uint8_t rpm= 0;
/*
 * Prototipos de funciones
 */
void setup(void);


/*
 *Interrupción
 */
void __interrupt() isr (void)
{
    
    
   if(PIR1bits.ADIF){
        //Interrupción
       if (ADCON0bits.CHS ==1){
             PWM_duty(0,ADRESH);
              valpot0 = ADRESH;
      
        }
        else if (ADCON0bits.CHS ==0){
            PWM_duty(1 ,ADRESH);
             valpot1 = ADRESH;
        }
      else if (ADCON0bits.CHS == 2)
            val = ADRESH;
            PIR1bits.ADIF =0;
       
    }
   if (INTCONbits.T0IF){
        //b==31 para un segundo
        // b == 305 para 10 segundos
        if (c<=100){
           c ++;  
          
        }
        
        if (c==100){
            rpm = a/2;
            a= 0;
            b= 0;
            c=0;
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
    setup();
    OSCCON = 0x70;    // set internal oscillator to 8MHz
     PWM_init (0,0,255);
    ADCON0bits.GO =1;
    
  __delay_ms(500);  // wait 2 seconds
 
  
 Lcd_Init();
 ADCON0bits.GO =1;
 char text[9];
    i=0;
    
    while(1)
    {
      
       if (ADCON0bits.GO ==0){
           for (i=0;i<=2;i++){
           ADCON0bits.CHS = i;
           ADCON0bits.GO =1;
        
           }
   
            
       }
       //Botones
       if (!PORTBbits.RB0){ //Verifica si la interrupción del puerto RB0 ha cambiado
          //  while (!RB0);
                a ++;  
               
        }
    if (!PORTBbits.RB1){
            //while (!RB1) ;
                b ++; 
                
                         
        }
       if (!PORTBbits.RB2){
           while (!RB2) ;
             PORTEbits.RE0 = 1;
       
        }
        if (!PORTBbits.RB3){
           while (!RB3) ;
           if( PORTEbits.RE1 == 0){
             PORTEbits.RE1 = 1; 
           }
           if( PORTEbits.RE1 == 1){
             PORTEbits.RE1 = 0; 
           }
        }
           
       
       //--------------------------------
       char s[20];
       char s2[20];
       char s3[20];
        sprintf(s, "%u", rpm);
        sprintf(s2, "%u", a);
        sprintf(s3, "%u", b);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(s);
    Lcd_Set_Cursor(1,4);
    Lcd_Write_String(s2); 
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String(s3);
       
       
       
    }
}
    
    

/*
 * Funciones
 */
void setup(void){
    ANSEL = 0b00000011;
    ANSELH = 0;
    
    TRISA = 0xFF;
    TRISB = 0b11111111;
    TRISD = 0;
    OPTION_REGbits.nRBPU =  0;
    TRISE = 0;
    
  
    WPUB = 0b1111;
    //PORTB = 0;
    //PORTC = 0;
    PORTD = 0;
    PORTE = 0;
   
   
    
    
    // Configuración del oscilador
    OSCCONbits.IRCF =   0b0111; //8MHz
    OSCCONbits.SCS = 1;
    
    // Configuración del ADC
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b01; //FOSC/32
    ADCON0bits.CHS = 0;
    ADCON0bits.ADON= 1;
    __delay_us(50);
       //Timer0 Registers Prescaler= 256 - TMR0 Preset = 0 - Freq = 30.52 Hz - Period = 0.032768 seconds
OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
OPTION_REGbits.T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
OPTION_REGbits.PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
OPTION_REGbits.PS2 = 1;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
OPTION_REGbits.PS1 = 1;
OPTION_REGbits.PS0 = 1;
TMR0 = 0;             // preset for timer register

    //Configuración de las interrupciones
    //Configuración para la interrupción del ADC      
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    //Configuración para la interrupción de los botones
    INTCONbits.RBIE = 0;
    INTCONbits.RBIF = 1;
    //Configuración para las interrupciones globales
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}
