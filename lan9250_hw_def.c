#include "spi.h"
#include <xc.h>
#include "lan9250_hw_def.h"


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

void lan9250_enable_interrupt_1(void){

}

void lan9250_enable_interrupt_2(void){
    
}

void lan9250_disable_interrupt_1(void){

}

void lan9250_disable_interrupt_2(void){
    
}