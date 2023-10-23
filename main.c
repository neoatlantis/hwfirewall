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
    
    spi2_enable();
    uart2_enable();
    printf("\n\r----\n\r");
    printf("NeoAtlantis Hardware Firewall\n\r");
    
    printf("Initialize LAN9250 at slot 1...");
    
    lan9250_init(1);
    
    
    while(1){
        WDTCONbits.WDTCLR = 1;
    }
    
    return;
}
