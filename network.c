#include <stdbool.h>
#include <string.h>
#include "lan9250_nic_def.h"
#include "lldp.h"

bool network_compare_mac_addr(uint8_t* buffer, uint8_t compared[6]){
    return 0 == memcmp(buffer, compared, 6);
}

void network_decide_on_packet(LAN9250Resource *nic){
    if(lldp_parse_ethernet_buffer(nic)) return;
    
}