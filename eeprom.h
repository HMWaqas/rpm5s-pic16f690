
void writeToEEPROM(unsigned char address, int data) {
   // INTCONbits.GIE = 0;      // Disable interrupts
    EEADR = address;
    EEDATA = (unsigned char)data;
   // EECON1bits.EEPGD = 0;    // Point to data memory
    //EECON1bits.CFGS = 0;     // Access EEPROM
    EECON1bits.WREN = 1;     // Enable write to EEPROM
    EECON2 = 0x55;           // Required sequence for write
    EECON2 = 0xAA;           // Required sequence for write
    EECON1bits.WR = 1;       // Start EEPROM write
    while (EECON1bits.WR == 1);  // Wait for write to complete
    EECON1bits.WREN = 0;     // Disable write to EEPROM
    //INTCONbits.GIE = 1;      // Enable interrupts
}


int readFromEEPROM(unsigned char address) {
   // INTCONbits.GIE = 0;      // Disable interrupts
    EEADR = address;
   // EECON1bits.EEPGD = 0;    // Point to data memory
    //EECON1bits.CFGS = 0;     // Access EEPROM
    EECON1bits.RD = 1;       // Start EEPROM read
    int data = EEDATA;       // Read the data
    //INTCONbits.GIE = 1;      // Enable interrupts
    return data;             // Return the read value
}