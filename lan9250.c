#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "spi.h"
#include "lan9250.h"
#include "lan9250_register_struct.h"

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

uint32_t lan9250_read_dword(LAN9250Resource nic, uint16_t addr){
    uint32_t ret;
    nic.deselect();
    // Use fast read, send 0x0b, addrH, addrL, and dummyByte.
    uint32_t firstDWord = (0x0b << 24) | (addr << 8);
    // As we begin with 4 bytes, we transfer in 32-bit mode
    spi2_set_mode_32();
    nic.select();
    // send first byte
    spi2_send_and_receive_new(firstDWord, false, false);
    ret = spi2_send_and_receive_new(0xAA, true, false);
    nic.deselect();
    return ret;
}

bool lan9250_write_dword(LAN9250Resource nic, uint16_t addr, uint32_t value){
    uint32_t ret;
    nic.deselect();
    uint32_t firstDWord = 0x00020000 | addr;
    spi2_set_mode_32();
    nic.select();
    // send first byte
    spi2_send_and_receive_new(firstDWord, false, false);
    ret = spi2_send_and_receive_new(value, true, false);
    nic.deselect();
    return true;
}

bool lan9250_read_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n){
    nic.deselect();
    
    if(n > bufferSize) return false;
    
    // Use fast read, send 0x0b, addrH, addrL, and dummyByte.
    uint32_t firstDWord = (0x0b << 24) | (addr << 8);
    // As we begin with 4 bytes, we transfer in 32-bit mode
    spi2_set_mode_32();
    
    
    nic.select();
    
    // send first byte
    spi2_send_and_receive_new(firstDWord, false, false);
    // retrieve rest bytes
    for(size_t i=0; i<n; i++){
        buffer[i] = spi2_send_and_receive_new(0xAA, true, false);
    }
    nic.deselect();
    return true;
}

bool lan9250_write_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n){
    nic.deselect();
    if(n > bufferSize) return false;

    uint32_t firstDWord = 0x00020000 | addr;
    spi2_set_mode_32();
    nic.select();

    spi2_send_and_receive_new(firstDWord, false, false);
    for(size_t i=0; i<n; i++){
        spi2_send_and_receive_new(buffer[i], false, true);
    }
    return true;
}


bool lan9250_test_byte(LAN9250Resource nic){
    return lan9250_read_dword(nic, 0x64) == 0x87654321;
}

bool lan9250_read_chip_id(LAN9250Resource nic){
    REG_ID_REV.value = lan9250_read_dword(nic, 0x50);
    return 0x9250 == REG_ID_REV.ID;
}

bool lan9250_read_hw_cfg(LAN9250Resource nic){
    REG_HW_CFG.value = lan9250_read_dword(nic, 0x74);
    return true;
}

bool lan9250_write_hw_cfg(LAN9250Resource nic){
    REG_HW_CFG.MUST_BE_0 = 0b0;
    REG_HW_CFG.MUST_BE_00 = 0b00;
    REG_HW_CFG.MUST_BE_1 = 0b1;
    return lan9250_write_dword(nic, 0x74, REG_HW_CFG.value);
}

void lan9250_init(char slot){
    LAN9250Resource nic = lan9250_resources[slot-1];
    printf("Activated LAN9250, waiting for ready...\n\r");
    
    while(!lan9250_test_byte(nic));
    printf("LAN9250 interface is ready.\n\r");
    
    while(!lan9250_read_chip_id(nic));
    printf("Confirming chip, type: %x, rev: %d\n\r", REG_ID_REV.ID, REG_ID_REV.REVISION);
    
    do{
        lan9250_read_hw_cfg(nic);
    } while(!REG_HW_CFG.READY);
    printf("Device reports in ready state. TX buffer size: %d kBytes.\n\r", REG_HW_CFG.TX_FIF_SZ);
}