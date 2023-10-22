#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "spi.h"
#include "lan9250.h"

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

LAN9250Resource lan9250_resources[2] = {
    {
        .buffer = {0},
        .select = lan9250_select_1,
        .deselect = lan9250_deselect_1
    },
    {
        .buffer = {0},
        .select = lan9250_select_2,
        .deselect = lan9250_deselect_2
    }
};


bool lan9250_read_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n){
    uint32_t b = 0;
    nic.deselect();
    
    if(n > bufferSize) return false;
    
    // Use fast read, send 0x0b, addrH, addrL, and dummyByte.
    uint32_t firstDWord = (0x0b << 24) | (addr << 8);
    // As we begin with 4 bytes, we transfer in 32-bit mode
    spi2_set_mode_32();
    
    
    nic.select();
    
    // send first byte
    spi2_send_and_receive_new(firstDWord, false);
    // retrieve rest bytes
    for(size_t i=0; i<n; i++){
        buffer[i] = spi2_send_and_receive_new(0xAA, true);
    }
    nic.deselect();
    return true;
}




bool lan9250_test_byte(LAN9250Resource nic){
    uint32_t buffer[1];
    if(false == lan9250_read_n_bytes(nic, buffer, sizeof(buffer), 0x64, 1)){
        return false;
    }
    return buffer[0] == 0x87654321;
}

void lan9250_init(char slot){
    LAN9250Resource nic = lan9250_resources[slot-1];
    printf("Activated LAN9250, waiting for ready...\n\r");
    
    while(!lan9250_test_byte(nic));
    printf("LAN9250 interface is ready.\n\r");
}