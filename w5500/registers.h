#ifndef _W5500_REGISTERS
#define _W5500_REGISTERS

#include <stdint.h>


#define W5500_CTRL_BYTE_RD_GENERAL      0b00000000
#define W5500_CTRL_BYTE_RD_SOCKET_REG(n)  (((n << 2) | 0b01) << 3) | 0b000
#define W5500_CTRL_BYTE_RD_SOCKET_TX(n)   (((n << 2) | 0b10) << 3) | 0b000
#define W5500_CTRL_BYTE_RD_SOCKET_RX(n)   (((n << 2) | 0b11) << 3) | 0b000
#define W5500_CTRL_BYTE_WR_GENERAL      0b00000100
#define W5500_CTRL_BYTE_WR_SOCKET_REG(n)  (((n << 2) | 0b01) << 3) | 0b100
#define W5500_CTRL_BYTE_WR_SOCKET_TX(n)   (((n << 2) | 0b10) << 3) | 0b100
#define W5500_CTRL_BYTE_WR_SOCKET_RX(n)   (((n << 2) | 0b11) << 3) | 0b100

typedef union {
    uint8_t value[2];
    struct {
        uint8_t H;
        uint8_t L;
    };
} W5500_uint16_t;


// General registers

typedef union {
    uint8_t value;
    struct {
        unsigned :1;
        unsigned FARP:1;
        unsigned :1;
        unsigned PPPoE:1;
        unsigned PB:1;
        unsigned WOL:1;
        unsigned :1;
        unsigned RST:1;
    };
} REG_GENERAL_MR;
#define ADDR_GENERAL_MR 0x0000, 1

typedef union {
    uint8_t value[4];
    NICIPv4Address addr;
} REG_GENERAL_GAR;
#define ADDR_GENERAL_GAR  0x0001, 4

typedef union {
    uint8_t value[4];
    NICIPv4Address addr;
} REG_GENERAL_SUBR;
#define ADDR_GENERAL_SUBR  0x0005, 4

typedef union {
    uint8_t value[6];
    NICMACAddress addr;
} REG_GENERAL_SHAR;
#define ADDR_GENERAL_SHAR  0x0009, 6

typedef union {
    uint8_t value[4];
    NICIPv4Address addr;
} REG_GENERAL_SIPR;
#define ADDR_GENERAL_SIPR  0x000F, 4


typedef union {
    struct W5500_GENERAL_REGISTERS_DETAILS {
        REG_GENERAL_MR MR;
        REG_GENERAL_GAR GAR;
        REG_GENERAL_SUBR SUBR;
        REG_GENERAL_SHAR SHAR;
        REG_GENERAL_SIPR SIPR;
        // ...
    } details;
    uint8_t buffer[sizeof(struct W5500_GENERAL_REGISTERS_DETAILS)];
} W5500_GENERAL_REGISTERS;




#define ADDR_GENERAL_VERSIONR   0x0039, 1



// Socket registers

typedef union {
    uint8_t value;
    struct {
        unsigned PROTOCOL:4;
        unsigned UCASTB:1;
        unsigned ND:1;
        unsigned BCASTB:1;
        unsigned MULTI:1;
    };
    struct {
        unsigned :4;
        unsigned MIP6B:1;
        unsigned MC:1;
        unsigned :1;
        unsigned MFEN:1;
    };
    struct {
        unsigned :5;
        unsigned MMB:1;
        unsigned :2;
    };
} REG_SOCKET_MR;
#define ADDR_SOCKET_MR  0x0000, 1

#define ADDR_SOCKET_CR  0x0001, 1

typedef union {
    uint8_t value;
    struct {
        unsigned CON:1;
        unsigned DISCON:1;
        unsigned RECV:1;
        unsigned TIMEOUT:1;
        unsigned SEND_OK:1;
        unsigned :3;
    };
} REG_SOCKET_IR;
#define ADDR_SOCKET_IR  0x0002, 1

#define ADDR_SOCKET_SR  0x0003, 1

typedef NICPort REG_SOCKET_PORT;
#define ADDR_SOCKET_PORT 0x0004, 2

typedef NICMACAddress REG_SOCKET_DHAR;
#define ADDR_SOCKET_DHAR 0x0006, 6

typedef NICIPv4Address REG_SOCKET_DIPR;
#define ADDR_SOCKET_DIPR 0x000C, 4

typedef NICPort REG_SOCKET_DPORT;
#define ADDR_SOCKET_DPORT 0x0010, 2

typedef W5500_uint16_t  REG_SOCKET_MSSR;
#define ADDR_SOCKET_MSSR 0x0012, 2

#define ADDR_SOCKET_TOS  0x0015, 1

#define ADDR_SOCKET_TTL  0x0016, 1

typedef W5500_uint16_t  REG_SOCKET_RXBUF_SIZE;
#define ADDR_SOCKET_RXBUF_SIZE  0x001E, 2

typedef W5500_uint16_t  REG_SOCKET_TXBUF_SIZE;
#define ADDR_SOCKET_TXBUF_SIZE  0x001F, 2

typedef W5500_uint16_t  REG_SOCKET_TX_FSR;
#define ADDR_SOCKET_TX_FSR  0x0020, 2

typedef W5500_uint16_t  REG_SOCKET_TX_RD;
#define ADDR_SOCKET_TX_RD  0x0022, 2

typedef W5500_uint16_t  REG_SOCKET_TX_WR;
#define ADDR_SOCKET_TX_WR  0x0024, 2

typedef W5500_uint16_t  REG_SOCKET_RX_RSR;
#define ADDR_SOCKET_RX_RSR  0x0026, 2

typedef W5500_uint16_t  REG_SOCKET_RX_RD;
#define ADDR_SOCKET_RX_RD  0x0028, 2

typedef W5500_uint16_t  REG_SOCKET_RX_WR;
#define ADDR_SOCKET_RX_WR  0x002A, 2

typedef union {
    uint8_t value;
    struct {
        unsigned CON:1;
        unsigned DISCON:1;
        unsigned RECV:1;
        unsigned TIMEOUT:1;
        unsigned SEND_OK:1;
        unsigned :3;
    };
} REG_SOCKET_IMR;
#define ADDR_SOCKET_IMR  0x002C, 1



// commands and status constants

#define W5500_SOCK_CMD_OPEN         0x01
#define W5500_SOCK_CMD_LISTEN       0x02
#define W5500_SOCK_CMD_CONNECT      0x04
#define W5500_SOCK_CMD_DISCON       0x08
#define W5500_SOCK_CMD_CLOSE        0x10
#define W5500_SOCK_CMD_SEND         0x20
#define W5500_SOCK_CMD_SEND_MAC     0x21
#define W5500_SOCK_CMD_SEND_KEEP    0x22
#define W5500_SOCK_CMD_RECV         0x40

#define W5500_SOCK_CLOSED           0x00
#define W5500_SOCK_INIT             0x13
#define W5500_SOCK_LISTEN           0x14
#define W5500_SOCK_ESTABLISHED      0x17
#define W5500_SOCK_CLOSE_WAIT       0x1C
#define W5500_SOCK_UDP              0x22
#define W5500_SOCK_MACRAW           0x42








#endif