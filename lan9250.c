#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "lan9250.h"
#include "lan9250_hw_def.h"
#include "lan9250_io.h"
#include "lan9250_register_struct.h"


LAN9250Resource lan9250_resources[2] = {
    {
        .buffer = {0},
        .select = lan9250_select_1,
        .deselect = lan9250_deselect_1,
        .enable_interrupt = lan9250_enable_interrupt_1,
        .disable_interrupt = lan9250_disable_interrupt_1
    },
    {
        .buffer = {0},
        .select = lan9250_select_2,
        .deselect = lan9250_deselect_2,
        .enable_interrupt = lan9250_enable_interrupt_2,
        .disable_interrupt = lan9250_disable_interrupt_2
    }
};



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

bool lan9250_read_mac_csr(LAN9250Resource nic, uint8_t addr, uint32_t *result){
    REG_MAC_CSR_CMD.CSRADDR = addr;
    REG_MAC_CSR_CMD.RW = 1; // 1-read, 0-write
    REG_MAC_CSR_CMD.HMAC_CSR_BUSY = 1;
    lan9250_write_dword(nic, 0xA4, REG_MAC_CSR_CMD.value);
    do{
        REG_MAC_CSR_CMD.value = lan9250_read_dword(nic, 0xA4);
    } while(REG_MAC_CSR_CMD.HMAC_CSR_BUSY);
    *result = lan9250_read_dword(nic, 0xA8);
    return true;
}

bool lan9250_write_mac_csr(LAN9250Resource nic, uint8_t addr, uint32_t *value){
    lan9250_write_dword(nic, 0xA8, *value); // write to HMAC_CSR_DATA
    REG_MAC_CSR_CMD.CSRADDR = addr;
    REG_MAC_CSR_CMD.RW = 0; // 1-read, 0-write
    REG_MAC_CSR_CMD.HMAC_CSR_BUSY = 1;
    lan9250_write_dword(nic, 0xA4, REG_MAC_CSR_CMD.value);
    do{
        REG_MAC_CSR_CMD.value = lan9250_read_dword(nic, 0xA4);
    } while(REG_MAC_CSR_CMD.HMAC_CSR_BUSY);
    return true;
}

void lan9250_init(char slot, LAN9250Config config){
    LAN9250Resource nic = lan9250_resources[slot-1];
    nic.disable_interrupt();
    
    printf("Activated LAN9250, waiting for ready...\n\r");
    
    while(!lan9250_test_byte(nic));
    printf("LAN9250 interface is ready.\n\r");
    
    while(!lan9250_read_chip_id(nic));
    printf("Confirming chip, type: %x, rev: %d\n\r", REG_ID_REV.ID, REG_ID_REV.REVISION);
    
    do{
        lan9250_read_hw_cfg(nic);
    } while(!REG_HW_CFG.READY);
    printf("Device reports in ready state. TX buffer size: %d kBytes.\n\r", REG_HW_CFG.TX_FIF_SZ);
    
    // set mac address
    
    lan9250_write_mac_csr(nic, 0x02, &config.MAC_ADDR_H);
    lan9250_write_mac_csr(nic, 0x03, &config.MAC_ADDR_L);
    
    lan9250_read_mac_csr(nic, 0x02, &REG_HMAC_ADDR_H.value);
    lan9250_read_mac_csr(nic, 0x03, &REG_HMAC_ADDR_L.value);
    printf(
        "Device MAC addr is %x:%x:%x:%x:%x:%x\n\r",
        REG_HMAC_ADDR_H.ADDR5,
        REG_HMAC_ADDR_H.ADDR4,
        REG_HMAC_ADDR_L.ADDR3,
        REG_HMAC_ADDR_L.ADDR2,
        REG_HMAC_ADDR_L.ADDR1,
        REG_HMAC_ADDR_L.ADDR0
    );
    
    nic.enable_interrupt();
}