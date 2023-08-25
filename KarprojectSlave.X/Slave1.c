/*
 * File:   Slave1.c
 * Author: diego
 *
 * Created on 17 de agosto de 2023, 04:32 PM
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
#include <xc.h>
#include <stdint.h>
#include "PWM.h"
#include "I2C.h"
#include <pic16f887.h>
#include "stdio.h"

#define _XTAL_FREQ 8000000
/*
 *Constantes
 */
int i;

/*
 *Variables
 */

uint8_t a= 0;
uint8_t b= 0;
int c= 0;
int s= 0;
uint8_t rpm= 0;
uint8_t valservo= 0;
/*
 * Prototipos de funciones
 */
void setup(void);


/*
 *Interrupción
 */
void __interrupt() isr (void)
{
    
    
   
   if (INTCONbits.T0IF){
        //b==31 para un segundo
        // b == 305 para 10 segundos
        if (c<=60){
           c ++;  
          
        }
        
        if (c==60){
            rpm = a/2;
            a= 0;
            b= 0;
            c=0;
        }
        INTCONbits.T0IF = 0;
        TMR0 = 0;
    
    }
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            
            z = SSPBUF;
            BF = 0;
            SSPBUF = rpm;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
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
      
       if(s==0){
           PORTD= 0;
       }
       if(s==1){
           PORTD= 6;
       }
         CCPR1L = (valservo>>1)+128; 
       //--------------------------------
     
  
       
       
       
    }
}
    
    

/*
 * Funciones
 */
void setup(void){
    ANSEL = 0;
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
    //Inicializacion del I2C
   I2C_Slave_Init(0x24);   
   
    
    
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
  
    //Configuración para la interrupción de los botones
    INTCONbits.RBIE = 0;
    INTCONbits.RBIF = 1;
    //Configuración para las interrupciones globales
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}
