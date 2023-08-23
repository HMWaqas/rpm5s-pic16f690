
#define _XTAL_FREQ 4000000 // 4 MHz crystal frequency

void initializeIOs(){
    
    // Configure all I/O ports as digital I/Os
    ANSEL = 0x00;  // Set ANSEL register to configure RA0 to RA3 as digital I/Os
    ANSELH = 0x00; // Set ANSELH register to configure RB4 to RB7 as digital I/Os

    
    // Configure all ports as outputs
    TRISA = 0b00111000; // Set TRISA register to configure all pins on Port A 
    TRISB = 0b00100000; // Set TRISB register to configure all pins on Port B 
    TRISC = 0b00100000; // Set TRISC register to configure all pins on Port C 
    
    
//    PORTA = 0x00;
//    PORTB = 0x00;
//    PORTC = 0x00;
    
    OSCCONbits.IRCF = 0b110; // Select 4MHz oscillator frequency
    

    
//    CMCONbits.CM0 = 1;   
//    CMCONbits.CM1 = 1;   // Comparators off
//    CMCONbits.CM2 = 1;   
    
//    CMCON0 = 0x07; //disable all comparators
//    CMCON1 = 0x07; //
}

void CCP1_Initialize() {
    // Configure CCP1 module for capture mode
    CCP1CONbits.CCP1M = 0b0101; // Capture mode, every rising edge
   // CCP1CONbits.P1M = 0b00;     // Single capture mode
    
    // Clear CCP1 interrupt flag
    PIR1bits.CCP1IF = 0;

    // Enable CCP1 interrupt
    PIE1bits.CCP1IE = 1;
    
    
    // Enable global interrupts
    INTCONbits.GIE = 1;      // Enable global interrupts
    INTCONbits.PEIE = 1;     // Enable peripheral interrupts

    
}

void TMR0_Initialize()
{
    
    //TMR0 = 0;                       //set the value for the timer register TMR0 to 0
    OPTION_REGbits.PSA = 0;         //pre-scaler assigned to timer0
    OPTION_REGbits.PS = 4;      // set pre-scaler PS:100 = 1:32
    OPTION_REGbits.T0CS = 0;        //set the timer mode to Timer mode
    //TMR0 = 1249;
    
    INTCONbits.T0IE = 1;            //enable the Timer0 interrupt
//    T0CONbits.TMR0ON = 1;           //start the timer
    
    INTCONbits.GIE = 1;             // Enable the Global Interrupt
}

void TMR1_Initialize() {
    
    // Configure TMR1 for 1ms interrupt interval
    T1CONbits.TMR1CS = 0;   // Use internal instruction cycle clock (Fosc/4)
    T1CONbits.T1CKPS = 0b11;  // Set prescaler to 1:8

    // Enable TMR1 interrupt
    PIE1bits.TMR1IE = 1;

    // Clear TMR1 interrupt flag
    PIR1bits.TMR1IF = 0;

    // Start TMR1
    TMR1 = 0;
    T1CONbits.TMR1ON = 1;
    
}


