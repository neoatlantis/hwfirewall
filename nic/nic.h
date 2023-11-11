#ifndef _NIC_H    /* Guard against multiple inclusion */
#define _NIC_H


typedef struct {
    void (*spi_exchange_register)(uint8_t *buf, uint16_t bufferSize);
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

typedef struct {
    uint8_t id;
    NICBasicHAL driver;
    
    NICMACAddress mac;
    NICIPv4Address ip;
    NICIPv4Address netmask;
    
    void (*init)(NIC*);
} NIC;




#endif /* _EXAMPLE_FILE_NAME_H */
