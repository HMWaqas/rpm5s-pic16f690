#include <xc.h>
#include "config.h"
#include "display.h"
#include "eeprom.h"

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA4/OSC2/CLKOUT and RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)


#define rpm_in PORTCbits.RC5
#define selector PORTAbits.RA3


#define FREQ_FACTOR 7500000 // (1*60) / (0.0000008)


void checkSelectorToggle();
int calculateRPM();
_Bool setNumberOfSamples(unsigned long, int);

unsigned long period;
uint8_t overflow_count=0; 
uint8_t numberOfSamples = 1; //variable to store second captured value
_Bool shouldUpdate;


const int PULSES_PER_REV[5]={3,4,5,6,1}; 



void main(void) {
    
    initializeIOs();
    CCP1_Initialize();
    TMR1_Initialize();
    TMR0_Initialize();
    
 
    selectedIndicator = readFromEEPROM(1); // Selection
    
    if(selectedIndicator == 255)
        selectedIndicator = 0;
    
    int delayCount = 0;
    

    while(1){
        
        if(++delayCount == 20000)
        {    
            delayCount = 0;
            
            
            if(overflow_count < 5)
            {
                if(shouldUpdate)
                    setDisplay(calculateRPM()); // update display value 
            }
            else
            { 
              period = 0;
              setDisplay(0);
            }
            
        }
        checkSelectorToggle(); 
    }
}


void __interrupt() ISR(void) {
    
    if (PIR1bits.CCP1IF)     // Check if CCP1 interrupt occurred
    {   
        // Capture the time period between consecutive pulses
        static unsigned int initialCapture=0;
        
        if (initialCapture == 0) {
            initialCapture = CCPR;
            overflow_count=0;
            
        } 
        else {
            
            period = CCPR;
            period = period - initialCapture;     
            if(overflow_count)
                period += ((unsigned long)overflow_count * 65535);
            
            
            shouldUpdate = setNumberOfSamples(period,numberOfSamples); //  if numberOfSamples changed
            
            initialCapture = 0;     // Reset capture1 value
            overflow_count=0;
            TMR1=0;
        }  
        
        PIR1bits.CCP1IF = 0;   // Clear CCP1 interrupt flag
    }
    else if (TMR1IF)  // Check if Timer1 overflow flag is set
    {
        overflow_count++;
        TMR1IF = 0;  // Clear the Timer1 overflow flag
    }
    else if (INTCONbits.T0IF) // Check if Timer0 overflow flag is set
    { 
        TMR0 = 150;          // Adjust initial value for Timer0 to get required frequency for multiplexing of 7_segment 
        latchDisplay();      // Call the latchDisplay function
        INTCONbits.T0IF = 0; // Clear the Timer0 interrupt flag
    }
}



void checkSelectorToggle(){
    static unsigned char lastState = 0;

    if (!selector && lastState) {
        lastState = 0;
        if(++selectedIndicator>4)
            selectedIndicator=0;
        writeToEEPROM(1,selectedIndicator);
    }
    else
        lastState = selector;
}


int calculateRPM()
{
    int rpm = (FREQ_FACTOR * numberOfSamples) / (PULSES_PER_REV[selectedIndicator] * period); // Calculate rpm
    return rpm;
}

_Bool setNumberOfSamples(unsigned long period, int samples){    
    period = period / samples;
    if(period < 9000) 
    {    
        numberOfSamples = 16;
        CCP1CONbits.CCP1M = 0b0111;
    }
    else if(period < 36000)
    {
        numberOfSamples = 4;
        CCP1CONbits.CCP1M = 0b0110;
    }
    else
    {
        numberOfSamples = 1;
        CCP1CONbits.CCP1M = 0b0101;
    }
    
    return numberOfSamples == samples;

}



