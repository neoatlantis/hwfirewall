#ifndef _LAN9250_OP_H
#define _LAN9250_OP_H

bool lan9250_test_byte(LAN9250Resource* nic);
bool lan9250_read_chip_id(LAN9250Resource* nic);
bool lan9250_read_hw_cfg(LAN9250Resource* nic);
bool lan9250_write_hw_cfg(LAN9250Resource* nic);
void lan9250_drop_packet(LAN9250Resource *nic, size_t bytesLength);



#endif