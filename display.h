
//Segments definition
#define seg_A PORTCbits.RC4
#define seg_B PORTCbits.RC6
#define seg_C PORTBbits.RB6
#define seg_D PORTBbits.RB4
#define seg_E PORTCbits.RC2
#define seg_F PORTCbits.RC3
#define seg_G PORTCbits.RC7

#define seg_ON 0
#define seg_OFF 1

//Display commons
#define com_1 PORTCbits.RC1
#define com_2 PORTCbits.RC0
#define com_3 PORTAbits.RA2
#define com_4 PORTAbits.RA1

#define com_indicator PORTAbits.RA0

#define com_ON 0
#define com_OFF 1




char display[4]={1,2,3,4};
int selectedIndicator = 0;



const unsigned char digits[] = {
    0xC0,  // 0
    0xF9,  // 1
    0xA4,  // 2
    0xB0,  // 3
    0x99,  // 4
    0x92,  // 5
    0x82,  // 6
    0xF8,  // 7
    0x80,  // 8
    0x90   // 9
};


const unsigned char indicators[] = {
    0b00010000,  // 0
    0b00001000,  // 1
    0b00000001,  // 2
    0b00100000,  // 3
    0b00000010,  // 4
};


void latchDisplay() {
    
    static int turn=0;
    unsigned char value;
    
    if(turn>3)
        value = indicators[selectedIndicator];  
    else
        value = digits[display[turn]];
            
    // Turn off all digits and indicators
    com_1 = com_2 = com_3 = com_4 = com_indicator= com_OFF;

    // Set segment values
    seg_A = (value >> 0) & 0x01;
    seg_B = (value >> 1) & 0x01;
    seg_C = (value >> 2) & 0x01;
    seg_D = (value >> 3) & 0x01;
    seg_E = (value >> 4) & 0x01;
    seg_F = (value >> 5) & 0x01;
    seg_G = (value >> 6) & 0x01;

    // Select the digit to display
    switch (turn++) {
        case 0:
            com_1 = com_ON;
            break;
        case 1:
            com_2 = com_ON;
            break;
        case 2:
            com_3 = com_ON;
            break;
        case 3:
            com_4 = com_ON;
            break;
        case 4:
            com_indicator = com_ON;
            turn = 0;
            break;
        default:
            break;
    }
}

void setDisplay(unsigned int value) {
    while (value >= 10000) {
        value = value / 10;
    }

    for (int i = 3; i >= 0; i--) {
        display[i] = value % 10;
        value /= 10;
    }
}

