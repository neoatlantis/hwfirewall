#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h>

#include "lan9250.h"
#include "lan9250_hw_def.h"
#include "lan9250_nic_def.h"
#include "lan9250_config.h"
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
    // On interrupt, refresh some register values for given nic, then exit the
    // ISR. All other jobs done in polling mechanism.
    
    /*
    nic->registers.INT_EN.RXSTOP_INT_EN = 1; // on receiver stopped
    nic->registers.INT_EN.RSFL_EN = 1; // receiver FIFO reaches level (default 0)
    nic->registers.INT_EN.RSFF_EN = 1; // receiver FIFO full
    nic->registers.INT_EN.TSFL_EN = 1;
    nic->registers.INT_EN.TSFF_EN = 1;
    nic->registers.INT_EN.TDFO_EN = 1;
    nic->registers.INT_EN.TDFA_EN = 1; // transmit FIFO available
    nic->registers.INT_EN.TXSO_EN = 1; 
     
    */
    
    
    lan9250_read_sysreg(INT_STS);
    
    if(
        nic->registers.INT_STS.RSFL ||
        nic->registers.INT_STS.RSFF
    ){
        lan9250_read_sysreg(RX_FIFO_INF);
    }
    
    if(
        nic->registers.INT_STS.TSFF ||
        nic->registers.INT_STS.TSFL ||
        nic->registers.INT_STS.TDFO ||
        nic->registers.INT_STS.TDFA ||
        nic->registers.INT_STS.TXSO
    ){
        lan9250_read_sysreg(TX_FIFO_INF);
    }
    
    // always clear INT_STS on chip afterwards
    uint32_t reset_value = 0xFFFFFFFF;
    lan9250_write_dword(nic, ADDR_INT_STS, &reset_value);
    nic->clear_interrupt();
}

void __ISR(_EXTERNAL_1_VECTOR, IPL7SOFT) on_external_interrupt_1(void){
    lan9250_on_external_interrupt(&lan9250_resources[0]);
}

void __ISR(_EXTERNAL_2_VECTOR, IPL7SOFT) on_external_interrupt_2(void){
    lan9250_on_external_interrupt(&lan9250_resources[1]);
}

////////////////////////////////////////////////////////////////////////////////





void lan9250_init_nic(char slot, LAN9250Config config){
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
    nic->registers.INT_EN.RXSTOP_INT_EN = 1; // on receiver stopped
    nic->registers.INT_EN.RSFL_EN = 1; // receiver FIFO reaches level (default 0)
    nic->registers.INT_EN.RSFF_EN = 1; // receiver FIFO full
    nic->registers.INT_EN.TSFL_EN = 1;
    nic->registers.INT_EN.TSFF_EN = 1;
    nic->registers.INT_EN.TDFO_EN = 1;
    nic->registers.INT_EN.TDFA_EN = 1; // transmit FIFO available
    nic->registers.INT_EN.TXSO_EN = 1;
    lan9250_write_sysreg(INT_EN);
    
    // enable receiver
    nic->registers.HMAC_CR.RXEN = 1;
    lan9250_write_mac_csr(nic, 0x01, &nic->registers.HMAC_CR.value);
    
    // enable host controller interrupt
    nic->enable_interrupt();
    
    printf("Interrupt for this NIC enabled.\n\r");
}

////////////////////////////////////////////////////////////////////////////////

void lan9250_job_for_nic(LAN9250Resource *rxnic, LAN9250Resource *txnic){
    uint16_t rx_info_status_used = rxnic->registers.RX_FIFO_INF.RXSUSED; // dwords
    uint16_t rx_info_data_used   = rxnic->registers.RX_FIFO_INF.RXDUSED; // bytes
    
    DWORD_RX_STATUS rx_status_error_mask = {
        .CRC_ERROR = 1,
        .ERROR_STATUS = 1,
        .FRAME_TOO_LONG = 1,
        .MII_ERROR = 1,
        .RX_WDT_ERROR = 1,
        .FILTERING_FAIL = 1
    };
    
    if(rx_info_status_used > 0){
        DWORD_RX_STATUS rx_status = lan9250_rx_status_fifo_pop(rxnic);
        if(rx_status.value & rx_status_error_mask.value != 0){
            // RX error detected, drop packet
            lan9250_drop_packet(rxnic, rx_status.LENGTH);
            printf("RX dropped a packet.\n\r");
        } else {
            if(lan9250_read_fifo(rxnic, rx_status.LENGTH)){
                // we have got a new packet in buffer. deal with it...
                for(uint16_t i=0; i<rxnic->bufferSize;i++){
                    printf("%2x ", rxnic->buffer[i]);
                }
                printf("\n\r");
            } else {
                // buffer not enough and got false, drop packet.
                lan9250_drop_packet(rxnic, rx_status.LENGTH);
                printf("RX skipped a packet.\n\r");
            }
        }
    }
    
    
    
    
    lan9250_read_sysreg(RX_FIFO_INF);
}

////////////////////////////////////////////////////////////////////////////////

void lan9250_init(void){
    LAN9250Config nic1_config = LAN9250_NIC1_CONFIG;
    LAN9250Config nic2_config = LAN9250_NIC2_CONFIG;
    lan9250_init_nic(1, nic1_config);
    lan9250_init_nic(2, nic2_config);
}

void lan9250_run_once(void){
    lan9250_job_for_nic(&lan9250_resources[0], &lan9250_resources[1]);
    lan9250_job_for_nic(&lan9250_resources[1], &lan9250_resources[0]);
}