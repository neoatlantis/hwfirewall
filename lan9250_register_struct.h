#include "stdint.h"

// 11.14.1 FIFO LEVEL INTERRUPT REGISTER (FIFO_INT)

union {
    uint32_t value;
    struct {
        unsigned RSFL: 8;
        unsigned :8;
        unsigned TSFL:8;
        unsigned TDFA:8;
    }
} REG_FIFO_INT_t;



union {
    uint32_t value;
    struct {
        unsigned :12;
        unsigned :2;
        unsigned :2;
        unsigned TX_FIF_SZ:4;
        unsigned MBO:1;
        unsigned :1;
        unsigned :3;
        unsigned AMDIX_EN:1;
        unsigned :1;
        unsigned READY:1;
        unsigned :4;
    }
} REG_HW_CFG_t;

// 11.14.5 RX FIFO INFORMATION REGISTER (RX_FIFO_INF)

union {
    uint32_t value;
    struct {
        unsigned RXDUSED:16;
        unsigned RXSUSED:8;
        unsigned :8;
    }
} REG_RX_FIFO_INF_t;

// 11.14.6 TX FIFO INFORMATION REGISTER (TX_FIFO_INF)

union {
    uint32_t value;
    struct {
        unsigned TXFREE:16;
        unsigned TXSUSED:8;
        unsigned :8;
    }
} REG_TX_FIFO_INF_t;

// 11.14.7 HOST MAC RX DROPPED FRAMES COUNTER REGISTER (RX_DROP)

union {
    uint32_t value;
    struct {
        unsigned RX_DFC:32;
    }
} REG_RX_DROP_t;

// 11.14.8 HOST MAC CSR INTERFACE COMMAND REGISTER (MAC_CSR_CMD)

union {
    uint32_t value;
    struct {
        unsigned CSRADDR:8;
        unsigned :22;
        unsigned RW:1;              // 0-write, 1-read
        unsigned HMAC_CSR_BUSY:1;
    }
} REG_MAC_CSR_CMD_t;

