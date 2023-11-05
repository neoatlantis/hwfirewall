#ifndef _LAN9250_CONFIG_H
#define _LAN9250_CONFIG_H

typedef struct {
    uint32_t MAC_ADDR_H;
    uint32_t MAC_ADDR_L;
} LAN9250Config;

#define LAN9250_NIC_BUFFER_SIZE 1550
#define LAN9250_NIC1_CONFIG  {\
    .MAC_ADDR_H = 0x0000AAAA,\
    .MAC_ADDR_L = 0xDEADBEEF\
}
#define LAN9250_NIC2_CONFIG  {\
    .MAC_ADDR_H = 0x0000CAFE,\
    .MAC_ADDR_L = 0xDEADBEEF\
}

#endif