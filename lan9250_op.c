#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "lan9250_io.h"
#include "lan9250_op.h"
#include "lan9250_nic_def.h"

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

void lan9250_drop_packet(LAN9250Resource *nic, size_t bytesLength){
    size_t i = 0;
    if(bytesLength >= 16){
        // use fast forward
        nic->registers.RX_DP_CTRL.RX_FFWD = 1;
        lan9250_write_sysreg(RX_DP_CTRL);
        do{
            lan9250_read_sysreg(RX_DP_CTRL);
        } while (nic->registers.RX_DP_CTRL.RX_FFWD);
    } else {
        uint32_t temp;
        for(i = 0; i < bytesLength; i += 4){
            //Perform standard PIO read operation
            lan9250_read_dword(nic, ADDR_RX_DATA_FIFO, &temp);
        }
    }
}

void lan9250_rx_dump(LAN9250Resource *nic){
    // stops the receiver, performs a dump, and resume the receiver
    // requires interrupt for updating INT_STS with RX_STOP information.
    nic->registers.HMAC_CR.RXEN = 0;
    lan9250_write_mac_csr(nic, 0x01, &nic->registers.HMAC_CR.value);
    // wait for receiver to stop
    while(!nic->registers.INT_STS.RXSTOP_INT);
    printf("Receiver stopped for dump.\n\r");
    lan9250_read_sysreg(RX_CFG);
    nic->registers.RX_CFG.RX_DUMP = 1;
    lan9250_write_sysreg(RX_CFG);
    // enable RX again
    nic->registers.HMAC_CR.RXEN = 1;
    lan9250_write_mac_csr(nic, 0x01, &nic->registers.HMAC_CR.value);
}

DWORD_RX_STATUS lan9250_rx_status_fifo_pop(LAN9250Resource *nic){
    DWORD_RX_STATUS ret;
    lan9250_read_dword(nic, ADDR_RX_STATUS_FIFO, &ret.value);
    return ret;    
}

DWORD_RX_STATUS lan9250_rx_status_fifo_peek(LAN9250Resource *nic){
    DWORD_RX_STATUS ret;
    lan9250_read_dword(nic, ADDR_RX_STATUS_FIFO_PEEK, &ret.value);
    return ret;    
}
