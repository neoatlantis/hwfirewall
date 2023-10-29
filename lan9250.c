#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>

#include "lan9250.h"
#include "lan9250_hw_def.h"
#include "lan9250_io.h"



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

void __ISR(_EXTERNAL_1_VECTOR, IPL7AUTO) on_external_interrupt_1(void){
    printf("Interrupt 1\n\r");
}

void __ISR(_EXTERNAL_2_VECTOR, IPL7AUTO) on_external_interrupt_2(void){
    printf("Interrupt 2\n\r");
}





////////////////////////////////////////////////////////////////////////////////

bool lan9250_test_byte(LAN9250Resource* nic){
    uint32_t value;
    lan9250_read_dword(nic, 0x64, &value);
    return 0x87654321 == value;
}

bool lan9250_read_chip_id(LAN9250Resource* nic){
    lan9250_read_sysreg(ID_REV);
    return 0x9250 == nic->registers.ID_REV.ID;
}

bool lan9250_read_hw_cfg(LAN9250Resource* nic){
    return lan9250_read_sysreg(HW_CFG);
}

bool lan9250_write_hw_cfg(LAN9250Resource* nic){
    nic->registers.HW_CFG.MUST_BE_0 = 0b0;
    nic->registers.HW_CFG.MUST_BE_00 = 0b00;
    nic->registers.HW_CFG.MUST_BE_1 = 0b1;
    return lan9250_write_dword(nic, 0x74, &nic->registers.HW_CFG.value);
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

void lan9250_init(char slot, LAN9250Config config){
    LAN9250Resource* nic = &lan9250_resources[slot-1];
    nic->disable_interrupt();
    
    printf("Activated LAN9250, waiting for ready...\n\r");
    
    while(!lan9250_test_byte(nic));
    printf("LAN9250 interface is ready.\n\r");
    
    // Reset the whole chip
    nic->registers.RESET_CTL.DIGITAL_RST = 1;
    lan9250_write_sysreg(RESET_CTL);
    
    // Wait for ready again
    while(!lan9250_test_byte(nic));
    
    while(!lan9250_read_chip_id(nic));
    printf("Confirming chip, type: %x, rev: %d\n\r", nic->registers.ID_REV.ID, nic->registers.ID_REV.REVISION);
    
    do{
        lan9250_read_hw_cfg(nic);
    } while(!nic->registers.HW_CFG.READY);
    printf("Device reports in ready state. TX buffer size: %d kBytes.\n\r", nic->registers.HW_CFG.TX_FIF_SZ);
    
    // set mac address
    
    lan9250_write_mac_csr(nic, 0x02, &config.MAC_ADDR_H);
    lan9250_write_mac_csr(nic, 0x03, &config.MAC_ADDR_L);
    
    lan9250_read_mac_csr(nic, 0x02, &nic->registers.HMAC_ADDR_H.value);
    lan9250_read_mac_csr(nic, 0x03, &nic->registers.HMAC_ADDR_L.value);
    printf(
        "Device MAC addr is %x:%x:%x:%x:%x:%x\n\r",
        nic->registers.HMAC_ADDR_H.ADDR5,
        nic->registers.HMAC_ADDR_H.ADDR4,
        nic->registers.HMAC_ADDR_L.ADDR3,
        nic->registers.HMAC_ADDR_L.ADDR2,
        nic->registers.HMAC_ADDR_L.ADDR1,
        nic->registers.HMAC_ADDR_L.ADDR0
    );
    
    // configure NIC external interrupt pin and enable interrupt on nic
    nic->registers.IRQ_CFG.IRQ_TYPE = 1; // push-pull
    nic->registers.IRQ_CFG.IRQ_POL = 0;  // active low
    nic->registers.IRQ_CFG.INT_DEAS = 1; // 10 us * 1
    nic->registers.IRQ_CFG.IRQ_EN = 1;   // enable irq
    lan9250_write_sysreg(IRQ_CFG);
    
    // enable specific interrupts on nic
    nic->registers.INT_EN.RSFL_EN = 1; // receiver fifo reaches level (default 0)
    nic->registers.INT_EN.RSFF_EN = 1; // receiver fifo full
    nic->registers.INT_EN.TDFA_EN = 1; // transmit fifo available
    
    // enable host controller interrupt
    nic->enable_interrupt();
}