#include "spi.h"
#include <xc.h>
#include "lan9250_hw_def.h"
#include "system_config.h"

void delay_us(unsigned int us){
    // Convert microseconds us into how many clock ticks it will take
    us *= SYSCLK / 1000000 / 2; // Core Timer updates every 2 ticks
    _CP0_SET_COUNT(0); // Set Core Timer count to 0
    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

void delay_ms(unsigned int ms){
    while ((ms--) > 0) delay_us(1000);
}

void lan9250_select_1(void){
    SPI_SLOT2_CS_BIT=1;
    SPI_SLOT1_CS_BIT=0;
}
void lan9250_deselect_1(void){
    SPI_SLOT1_CS_BIT = 1;
}
void lan9250_select_2(void){
    SPI_SLOT1_CS_BIT=1;
    SPI_SLOT2_CS_BIT=0;
}
void lan9250_deselect_2(void){
    SPI_SLOT2_CS_BIT = 1;
}

void lan9250_reset_1(void){
    TRISEbits.TRISE2 = 0; // set as output
    LATEbits.LATE2 = 0;
    delay_ms(500);
    LATEbits.LATE2 = 1;
    TRISEbits.TRISE2 = 1;
}

void lan9250_reset_2(void){
    TRISBbits.TRISB2 = 0; // set as output
    LATBbits.LATB2 = 0;
    delay_ms(500);
    LATBbits.LATB2 = 1;
    TRISBbits.TRISB2 = 1;
}

void lan9250_enable_interrupt_1(void){
    TRISEbits.TRISE8 = 1;
    
    IFS0bits.INT1IF = 0; // clear interrupt 1 flag
    IPC1bits.INT1IP = 7; // set interrupt priority to 7
    IPC1bits.INT1IS = 1; // set interrupt subpriority to 1
    INTCONbits.INT1EP = 0; // interrupt in falling edge mode
    IEC0bits.INT1IE = 1;   // enable interrupt
}

void lan9250_enable_interrupt_2(void){
    TRISEbits.TRISE9 = 1;
    
    IFS0bits.INT2IF = 0;
    IPC2bits.INT2IP = 7;
    IPC2bits.INT2IS = 2;
    INTCONbits.INT2EP = 0;
    IEC0bits.INT2IE = 1;
}

void lan9250_disable_interrupt_1(void){
    IEC0bits.INT1IE = 0;
    IFS0bits.INT1IF = 0;
}

void lan9250_disable_interrupt_2(void){
    IEC0bits.INT2IE = 0;
    IFS0bits.INT2IF = 0;
}

void lan9250_clear_interrupt_1(void){
    IFS0bits.INT1IF = 0;
}

void lan9250_clear_interrupt_2(void){
    IFS0bits.INT2IF = 0;
}