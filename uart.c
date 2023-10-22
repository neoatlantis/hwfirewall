#include "uart.h"
#include <xc.h>

void uart2_enable(){
    U2BRG = BRG_VAL;
    U2MODEbits.UEN = 0b00;
    U2MODEbits.ON = 1;
}

void putch(char ch){
    while(U2STAbits.UTXBF);             // Wait until buffer is empty
    U2TXREG = ch;                       // Transmit character
}