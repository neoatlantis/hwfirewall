
#ifndef _NETWORK_H    /* Guard against multiple inclusion */
#define _NETWORK_H

#include "lan9250_nic_def.h"

void network_decide_on_packet(LAN9250Resource*);
bool network_compare_mac_addr(uint8_t *, uint8_t[6]);

#endif
