#include <string.h>
#include <stdio.h>

#include "new.h"
#include "config.h"
#include "registers.h"
#include "spi.h"

W5500_GENERAL_REGISTERS w5500_general_registers[W5500_NUM_OF_NICS];

void w5500_init(NIC* nic){
    uint8_t tempbuf[4];
    W5500_GENERAL_REGISTERS *nicGR = &w5500_general_registers[nic->id];
    
    // Restart NIC
    nicGR->details.MR.RST = 1;
    do{
        w5500_spi_exchange_buffer(
            nic, &nicGR->details.MR.value,
            W5500_CTRL_BYTE_RD_GENERAL,
            ADDR_GENERAL_MR
        );
    } while(nicGR->details.MR.RST);
    
    // Check version
    w5500_spi_exchange_buffer(
        nic, tempbuf, W5500_CTRL_BYTE_RD_GENERAL, ADDR_GENERAL_VERSIONR);
    printf("Chip version: %d\n\r", tempbuf[0]);
    
    // Set MAC address
    memcpy(nicGR->details.SHAR.value, nic->mac.octet, 6);
    w5500_spi_exchange_buffer(
        nic, nicGR->details.SHAR.value,
        W5500_CTRL_BYTE_WR_GENERAL,
        ADDR_GENERAL_SHAR
    );
    
    // Set IP address
    memcpy(nicGR->details.SIPR.value, nic->ip_device.octet, 4);
    memcpy(nicGR->details.GAR.value, nic->ip_gateway.octet, 4);
    memcpy(nicGR->details.SUBR.value, nic->ip_netmask.octet, 4);
    w5500_spi_exchange_buffer(nic, nicGR->details.SIPR.value, W5500_CTRL_BYTE_WR_GENERAL, ADDR_GENERAL_SIPR);
    w5500_spi_exchange_buffer(nic, nicGR->details.GAR.value, W5500_CTRL_BYTE_WR_GENERAL, ADDR_GENERAL_GAR);
    w5500_spi_exchange_buffer(nic, nicGR->details.SUBR.value, W5500_CTRL_BYTE_WR_GENERAL, ADDR_GENERAL_SUBR);
    
    
}


void w5500_new(NIC* nic){
    nic->init = w5500_init;
}