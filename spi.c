#include <proc/p32mx460f512l.h>
#include "spi.h"
#include "stdbool.h"


void spi2_enable(){
    SPI2BUF;                  // clear SPI bus by reading from it
    SPI2BRG = 0;             
    SPI2STATbits.SPIROV = 0;  // clear the overflow bit
    SPI2CONbits.FRMEN   = 0;  // frame support disabled
    SPI2CONbits.MODE32  = 0;  // use 8 bit mode
    SPI2CONbits.MODE16  = 0;
    SPI2CONbits.MSTEN   = 1;  // user SPI in master mode
    SPI2CONbits.CKP     = 1;  // clock idle state is high
    
    // spi slot 1 cs pin
    SPI_SLOT1_CS_TRISBIT = 0;
    
    // spi slot 2 cs pin
    SPI_SLOT2_CS_TRISBIT = 0;
    
    // deselect all slot cs pins
    SPI_SLOT1_CS_BIT = 1;
    SPI_SLOT2_CS_BIT = 1;
    
    SPI2CONbits.ON      = 1;  // turn on SPI1           
}

void spi2_set_mode_16(){
    while(SPI2STATbits.SPIBUSY);
    SPI2CONbits.MODE32 = 0; SPI2CONbits.MODE16 = 1;
}

void spi2_set_mode_8(){
    while(SPI2STATbits.SPIBUSY);
    SPI2CONbits.MODE32 = 0; SPI2CONbits.MODE16 = 0;
}

void spi2_set_mode_32(){
    while(SPI2STATbits.SPIBUSY);
    SPI2CONbits.MODE32 = 1;
}


uint32_t spi2_send_and_receive_new(uint32_t c, bool invert_recv_endian, bool invert_tx_endian){
    //while(!SPI2STATbits.SPITBE);
    if(invert_tx_endian) c = revert_endian(c);
    
    SPI2BUF = c;
    while(!SPI2STATbits.SPIRBF);
    if(invert_recv_endian){
        uint32_t buf = SPI2BUF;
        buf = revert_endian(buf);
        return buf;
    }
    return SPI2BUF;
}

uint32_t spi2_receive_and_send_new(uint32_t c, bool invert_recv_endian){
    uint32_t buf;
    buf = SPI2BUF;
    SPI2BUF = c;
    if(invert_recv_endian){
        buf = ((buf & 0x000000FF) << 24) |
              ((buf & 0x0000FF00) << 8) |
              ((buf & 0x00FF0000) >> 8) |
              ((buf & 0xFF000000) >> 24);
    }
    while(!SPI2STATbits.SPITBE);
    return buf;
}

void spi2_exchange_buffer(uint8_t* buf, uint16_t bufferSize){
    for(uint16_t i=0; i<bufferSize; i++){
        SPI2BUF = buf[i];
        while(!SPI2STATbits.SPIRBF);
        buf[i] = SPI2BUF;
    }
}