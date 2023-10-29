/*
 * File:   main.c
 * Author: standard
 *
 * Created on 2023年10月18日, 下午10:27
 */

#include "config_bits.h"
#include <stdbool.h>
#include <stdio.h>
#include <xc.h>

#include "uart.h"
#include "spi.h"
#include "lan9250.h"
#include "system_config.h"


void main(void) {
    LAN9250Config nic1_config = {
        .MAC_ADDR_H = 0x0000AAAA,
        .MAC_ADDR_L = 0xDEADBEEF
    };
    LAN9250Config nic2_config = {
        .MAC_ADDR_H = 0xCAFEBABE,
        .MAC_ADDR_L = 0xDEADBEEF
    };
    
    
    spi2_enable();
    uart2_enable();
    printf("\n\r----\n\r");
    printf("NeoAtlantis Hardware Firewall\n\r");
    
    printf("Initialize LAN9250 at slot 1...");
    
    lan9250_init(1, nic1_config);
    lan9250_init(2, nic2_config);
    
    INTCONSET = _INTCON_MVEC_MASK;
    
    __builtin_enable_interrupts();
    
    while(1){
        WDTCONbits.WDTCLR = 1;
    }
    
    return;
}
