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
        .id = 1,
        .buffer = {0},
        .select = lan9250_select_1,
        .deselect = lan9250_deselect_1,
        .hardware_reset = lan9250_reset_1,
        .enable_interrupt = lan9250_enable_interrupt_1,
        .disable_interrupt = lan9250_disable_interrupt_1,
        .clear_interrupt = lan9250_clear_interrupt_1
    },
    {
        .id = 2,
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


void lan9250_refresh_status_registers(LAN9250Resource* nic){
    lan9250_read_sysreg(INT_STS);
    lan9250_read_sysreg(RX_FIFO_INF);
    lan9250_read_sysreg(TX_FIFO_INF);
}


void lan9250_on_external_interrupt(LAN9250Resource* nic){
    // On interrupt, refresh some register values for given nic, then exit the
    // ISR. All other jobs done in polling mechanism.
    
    lan9250_refresh_status_registers(nic);
    
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
    
    // enable, rx in promiscuous mode
    nic->registers.HMAC_CR.PRMS = 1;
    nic->registers.HMAC_CR.INVFILT = 0;
    nic->registers.HMAC_CR.RXALL = 1;
    // enable rx and tx
    nic->registers.HMAC_CR.RXEN = 1;
    nic->registers.HMAC_CR.TXEN = 1;
    // full-duplex mode
    nic->registers.HMAC_CR.FDPX = 1;
    lan9250_write_mac_csr(nic, 0x01, &nic->registers.HMAC_CR.value);
    
    // enable transmitter 2
    lan9250_read_sysreg(TX_CFG);
    nic->registers.TX_CFG.TX_ON = 1;
    nic->registers.TX_CFG.TXSAO = 1; // allow tx status fifo overrun
    lan9250_write_sysreg(TX_CFG);
    
    // enable host controller interrupt
    nic->enable_interrupt();
    
    printf("Interrupt for this NIC enabled.\n\r");
}

////////////////////////////////////////////////////////////////////////////////

uint16_t global_packet_tag = 0;

void lan9250_job_for_nic(LAN9250Resource *rxnic, LAN9250Resource *txnic){
    bool anything_done = false;
    
    lan9250_refresh_status_registers(rxnic);
    lan9250_refresh_status_registers(txnic);
    
    uint16_t rx_info_status_used = rxnic->registers.RX_FIFO_INF.RXSUSED; // dwords
    uint16_t tx_info_status_used = txnic->registers.TX_FIFO_INF.TXSUSED; // dwords
    uint16_t rx_info_data_used   = rxnic->registers.RX_FIFO_INF.RXDUSED; // bytes
    uint16_t tx_info_data_free   = txnic->registers.TX_FIFO_INF.TXFREE;  // bytes
    
    
    DWORD_RX_STATUS rx_status_error_mask = {
        .CRC_ERROR = 1,
        .ERROR_STATUS = 1,
        .FRAME_TOO_LONG = 1,
        .MII_ERROR = 1,
        .RX_WDT_ERROR = 1,
        .FILTERING_FAIL = 1
    };
    
    if(rx_info_status_used > 0){
        anything_done = true;
        
        DWORD_RX_STATUS rx_status = lan9250_rx_status_fifo_pop(rxnic);
        if(rx_status.value & rx_status_error_mask.value != 0){
            // RX error detected, drop packet
            lan9250_drop_packet(rxnic, rx_status.LENGTH);
            printf("RDP1,");
        } else {
            if(rx_status.LENGTH > rx_info_data_used){
                // strange?
                lan9250_drop_packet(rxnic, rx_status.LENGTH);
            } else if(lan9250_read_fifo(rxnic, rx_status.LENGTH)){
                // we have got a new packet in buffer. deal with it...
                /*for(uint16_t i=0; i<rxnic->bufferSize;i++){
                    printf("%2x ", rxnic->buffer[i]);
                }
                printf("\n\r");*/
                printf("RSUC,");
            } else {
                // buffer not enough and got false, drop packet.
                lan9250_drop_packet(rxnic, rx_status.LENGTH);
                printf("RSK1,");
            }
        }
    }
    
    if(tx_info_status_used > 0){
        anything_done = true;
        
        DWORD_TX_STATUS tx_status = lan9250_tx_status_fifo_pop(txnic);
        if(tx_status.ERROR_STATUS){
            printf("TXE=%hx,", tx_status.value);
        } else {
            printf("TXOK,");
        }
    }
    
    // TODO shutdown host interrupt for non-interrupted transmission
    
    if(rxnic->bufferSize > 0){ // if we have anything to send
        anything_done = true;
        
        uint16_t tx_fifo_requirement = rxnic->bufferSize + 8; // plus 2 dwords
        if(tx_fifo_requirement <= tx_info_data_free){
            // send the buffer from rx to tx
            DWORD_TX_COMMAND_A txcmd_a = {
                .BUFFER_SIZE = rxnic->bufferSize,
                .BUFFER_END_ALIGNMENT = 0b00, // 4 byte
                .DATA_START_OFFSET = 0,
                .FIRST_SEGMENT = 1,
                .LAST_SEGMENT = 1
            };
            DWORD_TX_COMMAND_B txcmd_b = {
                .ADD_CRC_DISABLE = 0,
                .PACKET_LENGTH = rxnic->bufferSize,
                .PACKET_TAG = (global_packet_tag++),
            };
            lan9250_write_dword(txnic, ADDR_TX_DATA_FIFO, &txcmd_a.value);
            lan9250_write_dword(txnic, ADDR_TX_DATA_FIFO, &txcmd_b.value);
            lan9250_write_fifo(txnic, rxnic);
            rxnic->bufferSize = 0;
            printf("TXD,");
        } else {
            // tx nic buffer full, cannot send
            printf("NOTX,");
            // dump txd and txs fifo
            lan9250_tx_dump(txnic);
        }
    }
    
    
    if(anything_done){
        printf("/%d%d\n\r", rxnic->id, txnic->id);
    }
    
//    lan9250_read_sysreg(RX_FIFO_INF);
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