#ifndef _NIC_H    /* Guard against multiple inclusion */
#define _NIC_H

#include <stdint.h>

#define NIC_UDP_MAX_BUFFER 2048



typedef struct {
    void (*spi_select)(void);
    void (*spi_exchange_register)(uint8_t *buf, uint16_t bufferSize);
    void (*spi_deselect)(void);
    void (*irq_enable)(void);
    void (*irq_disable)(void);
    void (*hw_reset)(void);
} NICBasicHAL;

typedef union {
    uint8_t octet[6]; // octet[0]:octet[1]:octet[2]:octet[3]:octet[4]:octet[5]
    struct {
        uint8_t octet0;
        uint8_t octet1;
        uint8_t octet2;
        uint8_t octet3;
        uint8_t octet4;
        uint8_t octet5;
    };
} NICMACAddress;

typedef union {
    uint8_t octet[4]; // octet[0]:octet[1]:octet[2]:octet[3]
    struct {
        uint8_t octet0;
        uint8_t octet1;
        uint8_t octet2;
        uint8_t octet3;
    };
} NICIPv4Address;

typedef union {
    uint8_t octet[2];
    struct {
        uint8_t octetH;
        uint8_t octetL;
    };
} NICPort;

typedef struct NIC {
    uint8_t id;
    NICBasicHAL driver;
    
    NICMACAddress mac;
    
    NICIPv4Address ip_gateway;
    NICIPv4Address ip_device;
    NICIPv4Address ip_netmask;
    
    void (*init)(struct NIC*);
} NIC;

typedef struct {
    NICIPv4Address src_addr;
    NICPort        src_port;
    NICIPv4Address dst_addr;
    NICPort        dst_port;
    uint8_t        buffer[NIC_UDP_MAX_BUFFER];
    uint16_t       bufferSize;
} NICUDPPacket;


#endif
