#include <stdint.h>
#include "lan9250.h"
#include "spi.h"
#include "lan9250_io.h"
#include "lan9250_nic_def.h"
#include "lan9250_config.h"

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


bool lan9250_read_mac_csr(LAN9250Resource* nic, uint8_t addr, uint32_t *result){
    nic->registers.MAC_CSR_CMD.CSRADDR = addr;
    nic->registers.MAC_CSR_CMD.RW = 1; // 1-read, 0-write
    nic->registers.MAC_CSR_CMD.HMAC_CSR_BUSY = 1;
    lan9250_write_sysreg(MAC_CSR_CMD);
    do{
        lan9250_read_sysreg(MAC_CSR_CMD);
    } while(nic->registers.MAC_CSR_CMD.HMAC_CSR_BUSY);
    return lan9250_read_dword(nic, 0xA8, result);
}

bool lan9250_write_mac_csr(LAN9250Resource* nic, uint8_t addr, uint32_t *value){
    lan9250_write_dword(nic, ADDR_MAC_CSR_DATA, value); // write to HMAC_CSR_DATA
    nic->registers.MAC_CSR_CMD.CSRADDR = addr;
    nic->registers.MAC_CSR_CMD.RW = 0; // 1-read, 0-write
    nic->registers.MAC_CSR_CMD.HMAC_CSR_BUSY = 1;
    lan9250_write_sysreg(MAC_CSR_CMD);
    do{
        lan9250_read_sysreg(MAC_CSR_CMD);
    } while(nic->registers.MAC_CSR_CMD.HMAC_CSR_BUSY);
    return true;
}



bool lan9250_read_fifo(LAN9250Resource* nic, uint16_t length){
    nic->deselect();
    if(length > LAN9250_NIC_BUFFER_SIZE) return false;
    
    spi2_set_mode_8(); // use 8 bytes mode
    
    nic->select();
    
    // send initial bytes
    spi2_send_and_receive_new(0x0b, false, false);
    spi2_send_and_receive_new((uint8_t)(ADDR_RX_DATA_FIFO >> 8), false, false);
    spi2_send_and_receive_new((uint8_t)(ADDR_RX_DATA_FIFO & 0xFF), false, false);
    spi2_send_and_receive_new(0xAA, false, false); // dummy byte
    
    // retrieve rest bytes
    for(size_t i=0; i<length; i++){
        nic->buffer[i] = spi2_send_and_receive_new(0xAA, false, false);
    }
    nic->bufferSize = length;
    nic->deselect();
    return true;
}

/*bool lan9250_write_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n){
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
}*/