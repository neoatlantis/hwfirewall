
#ifndef _NETWORK_H    /* Guard against multiple inclusion */
#define _NETWORK_H

#include "lan9250_nic_def.h"

void network_decide_on_packet(LAN9250EthernetPacket *packet, struct LAN9250EthernetPacketDecisions*);



#endif /* _EXAMPLE_FILE_NAME_H */
