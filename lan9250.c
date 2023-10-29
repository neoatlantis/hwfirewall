#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>

#include "lan9250.h"
#include "lan9250_hw_def.h"
#include "lan9250_nic_def.h"
#include "lan9250_io.h"
#include "lan9250_op.h"



LAN9250Resource lan9250_resources[2] = {
    {
        .buffer = {0},
        .select = lan9250_select_1,
        .deselect = lan9250_deselect_1,
        .hardware_reset = lan9250_reset_1,
        .enable_interrupt = lan9250_enable_interrupt_1,
        .disable_interrupt = lan9250_disable_interrupt_1,
        .clear_interrupt = lan9250_clear_interrupt_1
    },
    {
        .buffer = {0},
        .select = lan9250_select_2,
        .deselect = lan9250_deselect_2,
        .hardware_reset = lan9250_reset_2,
        .enable_interrupt = lan9250_enable_interrupt_2,
        .disable_interrupt = lan9250_disable_interrupt_2,
        .clear_interrupt = lan9250_clear_interrupt_2
    }
};

////////////////////////////////////////////////////////////////////////////////



void lan9250_on_external_interrupt(LAN9250Resource* nic){
    lan9250_read_sysreg(INT_STS);
    printf("interrupt! %x", nic->registers.INT_STS.value);
    
    if(nic->registers.INT_STS.RSFL){
        // receiving FIFO level reached.
        lan9250_read_sysreg(RX_FIFO_INF);
        printf(
            "RX FIFO usage: status %d dwords, data %d dwords",
            nic->registers.RX_FIFO_INF.RXSUSED,
            nic->registers.RX_FIFO_INF.RXDUSED
        );
    }
    
    
    
    nic->registers.INT_STS.value = 0xFFFFFFFF; // write clear all interrupt flags
    lan9250_write_sysreg(INT_STS);
    lan9250_read_sysreg(INT_STS);
    printf("interrupt sts after clear: %x", nic->registers.INT_STS.value);
    nic->clear_interrupt();
}

void __ISR(_EXTERNAL_1_VECTOR, IPL7SOFT) on_external_interrupt_1(void){
    lan9250_on_external_interrupt(&lan9250_resources[0]);
}

void __ISR(_EXTERNAL_2_VECTOR, IPL7SOFT) on_external_interrupt_2(void){
    lan9250_on_external_interrupt(&lan9250_resources[1]);
}

////////////////////////////////////////////////////////////////////////////////





void lan9250_init(char slot, LAN9250Config config){
    LAN9250Resource* nic = &lan9250_resources[slot-1];
    nic->disable_interrupt();
    printf("HW reset...");
    nic->hardware_reset();
    printf("done.\n\r");
    
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
    nic->registers.IRQ_CFG.IRQ_TYPE = 1; 
    nic->registers.IRQ_CFG.IRQ_POL = 0;  // active low
    nic->registers.IRQ_CFG.INT_DEAS = 1; // 10 us * 1
    nic->registers.IRQ_CFG.IRQ_EN = 1;   // enable IRQ
    lan9250_write_sysreg(IRQ_CFG);
    
    // enable specific interrupts on nic
    nic->registers.INT_EN.RSFL_EN = 1; // receiver FIFO reaches level (default 0)
    nic->registers.INT_EN.RSFF_EN = 1; // receiver FIFO full
    nic->registers.INT_EN.TDFA_EN = 1; // transmit FIFO available
    lan9250_write_sysreg(INT_EN);
    
    // enable receiver
    nic->registers.HMAC_CR.RXEN = 1;
    lan9250_write_mac_csr(nic, 0x01, &nic->registers.HMAC_CR.value);
    
    // enable host controller interrupt
    nic->enable_interrupt();
    
    printf("Interrupt for this NIC enabled.\n\r");
}