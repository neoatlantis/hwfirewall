#include <stdint.h>
#include "lan9250.h"
#include "spi.h"

bool lan9250_read_dword(LAN9250Resource* nic, uint16_t addr, uint32_t* ret){
    nic->deselect();
    // Use fast read, send 0x0b, addrH, addrL, and dummyByte.
    uint32_t firstDWord = (0x0b << 24) | (addr << 8);
    // As we begin with 4 bytes, we transfer in 32-bit mode
    spi2_set_mode_32();
    nic->select();
    // send first byte
    spi2_send_and_receive_new(firstDWord, false, false);
    *ret = spi2_send_and_receive_new(0xAA, true, false);
    nic->deselect();
    return true;
}

bool lan9250_write_dword(LAN9250Resource* nic, uint16_t addr, uint32_t* value){
    uint32_t ret;
    nic->deselect();
    uint32_t firstDWord = 0xFF020000 | addr;
    spi2_set_mode_32();
    nic->select();
    // send first byte
    spi2_send_and_receive_new(firstDWord, false, false);
    spi2_send_and_receive_new(*value, false, true);
    nic->deselect();
    return true;
}

/*bool lan9250_read_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n){
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

    uint32_t firstDWord = 0xFF020000 | addr;
    spi2_set_mode_32();
    nic.select();

    spi2_send_and_receive_new(firstDWord, false, false);
    for(size_t i=0; i<n; i++){
        spi2_send_and_receive_new(buffer[i], false, true);
    }
    return true;
}
*/