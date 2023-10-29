#ifndef _LAN9250_OP_H
#define _LAN9250_OP_H

#include "lan9250_nic_def.h"
#include "lan9250_register_struct.h"

bool lan9250_test_byte(LAN9250Resource* nic);
bool lan9250_read_chip_id(LAN9250Resource* nic);
bool lan9250_read_hw_cfg(LAN9250Resource* nic);
bool lan9250_write_hw_cfg(LAN9250Resource* nic);
void lan9250_drop_packet(LAN9250Resource *nic, size_t bytesLength);
void lan9250_rx_dump(LAN9250Resource *nic);

DWORD_RX_STATUS lan9250_rx_status_fifo_pop(LAN9250Resource *nic);
DWORD_RX_STATUS lan9250_rx_status_fifo_peek(LAN9250Resource *nic);


#endif