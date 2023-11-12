/*
 * File:   main.c
 * Author: standard
 *
 * Created on 2023年10月18日, 下午10:27
 */

#include "config_bits.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>

#include "uart.h"
#include "spi.h"
#include "nic/nic.h"
#include "w5500/new.h"
#include "w5500/udp_socket.h"
#include "system_config.h"

NIC nic[2];

void w5500_nic0_select(){
    spi2_set_mode_8();
    SPI_SLOT1_CS_BIT = 0;
    SPI_SLOT2_CS_BIT = 1;
}

void w5500_nic1_select(){
    spi2_set_mode_8();
    SPI_SLOT1_CS_BIT = 1;
    SPI_SLOT2_CS_BIT = 0;
}

void w5500_nics_deselect(){
    SPI_SLOT1_CS_BIT = SPI_SLOT2_CS_BIT = 1;
}


void main(void) {
    spi2_enable();
    uart2_enable();
    printf("\n\r----\n\r");
    printf("NeoAtlantis Hardware Firewall\n\r");
    
    nic[0].id = 0;
    memcpy(&nic[0].mac.octet,       &(uint8_t[6]){ 0x02, 0xDE, 0xAD, 0xBE, 0xEF, 0x00 }, 6);
    memcpy(&nic[0].ip_device.octet, &(uint8_t[4]){ 192, 168, 144, 1 }, 4);
    memcpy(&nic[0].ip_netmask.octet,&(uint8_t[4]){ 255, 255, 255, 0 }, 4);
    nic[0].driver.spi_select = w5500_nic0_select;
    nic[0].driver.spi_deselect = w5500_nics_deselect;
    nic[0].driver.spi_exchange_register = spi2_exchange_buffer;
    
    nic[1].id = 1;
    memcpy(&nic[1].mac.octet,       &(uint8_t[6]){ 0x02, 0xDE, 0xAD, 0xBE, 0xEF, 0x01 }, 6);
    memcpy(&nic[1].ip_device.octet, &(uint8_t[4]){ 192, 168, 144, 2 }, 4);
    memcpy(&nic[1].ip_netmask.octet,&(uint8_t[4]){ 255, 255, 255, 0 }, 4);
    nic[1].driver.spi_select = w5500_nic1_select;
    nic[1].driver.spi_deselect = w5500_nics_deselect;
    nic[1].driver.spi_exchange_register = spi2_exchange_buffer;
    
    w5500_new(&nic[0]);
    w5500_new(&nic[1]);
    
    printf("Initialize W5500 at slot 1...\n\r");
    nic[0].init(&nic[0]);
    
    printf("Opening UDP port 51820 at slot 0...\n\r");
    w5500_open_udp_socket(&nic[0], 0, 51820);
    
    printf("Initialize W5500 at slot 2...\n\r");
    nic[1].init(&nic[1]);
    
    printf("Opening UDP port 9999 at slot 1...\n\r");
    
    w5500_open_udp_socket(&nic[1], 0, 9999);
    
    printf("Init done.\n\r");
    
    INTCONSET = _INTCON_MVEC_MASK;
    
    __builtin_enable_interrupts();
    
    uint8_t buffer[2048];
    
    while(1){
        WDTCONbits.WDTCLR = 1;
        NICUDPPacket udpp = w5500_udp_socket_read(&nic[0], 0);
        if(udpp.bufferSize > 0){
            printf(
                "Read %d bytes from %d.%d.%d.%d:%d\n\r", udpp.bufferSize,
                    udpp.src_addr.octet0,
                    udpp.src_addr.octet1,
                    udpp.src_addr.octet2,
                    udpp.src_addr.octet3,
                    (udpp.src_port.octetH << 8 | udpp.src_port.octetL)
            );
            
            // modify udpp for reply, test purpose.
            memcpy(udpp.dst_addr.octet, udpp.src_addr.octet, 4);
            memcpy(udpp.dst_port.octet, udpp.src_port.octet, 2);
            w5500_udp_socket_send(nic, 0, &udpp);
            
            udpp.bufferSize = 0;
        }
    }
    
    return;
}
