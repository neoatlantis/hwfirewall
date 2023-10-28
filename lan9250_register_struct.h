#ifndef _LAN9250_REGISTER_STRUCT
#define _LAN9250_REGISTER_STRUCT

#include "stdint.h"

// 8.3.1 INTERRUPT CONFIGURATION REGISTER (IRQ_CFG)

typedef union {
    uint32_t value;
    struct {
        unsigned IRQ_TYPE:1;
        unsigned IRQ_CLK_SELECT:1;
        unsigned :2;
        unsigned IRQ_POL:1;
        unsigned :3;
        unsigned IRQ_EN:1;
        unsigned :3;
        unsigned IRQ_INT:1;
        unsigned INT_DEAS_STS:1;
        unsigned INT_DEAS_CLR:1;
        unsigned :9;
        unsigned INT_DEAS:8;
    };
} REG_IRQ_CFG;

// 8.3.2 INTERRUPT STATUS REGISTER (INT_STS)

typedef union {
    uint32_t value;
    struct {
        unsigned :3;
        unsigned RSFL:1;
        unsigned RSFF:1;
        unsigned :1;
        unsigned RXDF_INT:1;
        unsigned TSFL:1;
        unsigned TSFF:1;
        unsigned TDFA:1;
        unsigned TDFO:1;
        unsigned :1;
        unsigned GPIO:1;
        unsigned TXE_INT:1;
        unsigned RXE_INT:1;
        unsigned RWT_INT:1;
        unsigned TXSO:1;
        unsigned PME_INT:1;
        unsigned :1;
        unsigned GPT_INT:1;
        unsigned RXD_INT:1;
        unsigned TIOC_INT:1;
        unsigned :1;
        unsigned RXDFH_INT:1;
        unsigned RXSTOP_INT:1;
        unsigned TXSTOP_INT:1;
        unsigned PHY_INT:1;
        unsigned :2;
        unsigned _1588_EVNT:1;
        unsigned READY:1;
        unsigned SW_INT:1;
    };
} REG_INT_STS;

// 8.3.3 INTERRUPT ENABLE REGISTER (INT_EN)

typedef union {
    uint32_t value;
    struct {
        unsigned :3;
        unsigned RSFL_EN:1;
        unsigned RSFF_EN:1;
        unsigned :1;
        unsigned RXDF_INT_EN:1;
        unsigned TSFL_EN:1;
        unsigned TSFF_EN:1;
        unsigned TDFA_EN:1;
        unsigned TDFO_EN:1;
        unsigned :1;
        unsigned GPIO_EN:1;
        unsigned TXE_INT_EN:1;
        unsigned RXE_INT_EN:1;
        unsigned RWT_INT_EN:1;
        unsigned TXSO_EN:1;
        unsigned PME_INT_EN:1;
        unsigned :1;
        unsigned GPT_INT_EN:1;
        unsigned RXD_INT_EN:1;
        unsigned TIOC_INT_EN:1;
        unsigned :1;
        unsigned RXDFH_INT_EN:1;
        unsigned RXSTOP_INT_EN:1;
        unsigned TXSTOP_INT_EN:1;
        unsigned PHY_INT_EN:1;
        unsigned :2;
        unsigned _1588_EVNT_EN:1;
        unsigned READY_EN:1;
        unsigned SW_INT_EN:1;
    };
} REG_INT_EN;

// 17.1 Miscellaneous System Configuration & Status Registers

// 17.1.1 CHIP ID AND REVISION

typedef union {
    uint32_t value;
    struct {
        unsigned REVISION: 16;
        unsigned ID: 16;
    };
} REG_ID_REV;

// 17.1.3 HARDWARE CONFIGURATION REGISTER (HW_CFG)

typedef union {
    uint32_t value;
    struct {
        unsigned :12;
        unsigned MUST_BE_00:2;
        unsigned :2;
        unsigned TX_FIF_SZ:4;
        unsigned MUST_BE_1:1;
        unsigned MUST_BE_0:1;
        unsigned :3;
        unsigned AMDIX_EN:1;
        unsigned :1;
        unsigned READY:1;
        unsigned :4;
    };
} REG_HW_CFG;

// 11.14.1 FIFO LEVEL INTERRUPT REGISTER (FIFO_INT)

typedef union {
    uint32_t value;
    struct {
        unsigned RSFL: 8;
        unsigned :8;
        unsigned TSFL:8;
        unsigned TDFA:8;
    };
} REG_FIFO_INT;





// 11.14.5 RX FIFO INFORMATION REGISTER (RX_FIFO_INF)

typedef union {
    uint32_t value;
    struct {
        unsigned RXDUSED:16;
        unsigned RXSUSED:8;
        unsigned :8;
    };
} REG_RX_FIFO_INF;

// 11.14.6 TX FIFO INFORMATION REGISTER (TX_FIFO_INF)

typedef union {
    uint32_t value;
    struct {
        unsigned TXFREE:16;
        unsigned TXSUSED:8;
        unsigned :8;
    };
} REG_TX_FIFO_INF;

// 11.14.7 HOST MAC RX DROPPED FRAMES COUNTER REGISTER (RX_DROP)

typedef union {
    uint32_t value;
    struct {
        unsigned RX_DFC:32;
    };
} REG_RX_DROP;

// 11.14.8 HOST MAC CSR INTERFACE COMMAND REGISTER (MAC_CSR_CMD)

typedef union {
    uint32_t value;
    struct {
        unsigned CSRADDR:8;
        unsigned :22;
        unsigned RW:1;              // 0-write, 1-read
        unsigned HMAC_CSR_BUSY:1;
    };
} REG_MAC_CSR_CMD;



// 11.15.2 HOST MAC ADDRESS HIGH REGISTER (HMAC_ADDRH)

typedef union {
    uint32_t value;
    struct {
        unsigned ADDR4:8;
        unsigned ADDR5:8;
        unsigned :16;
    };
} REG_HMAC_ADDR_H;

// 11.15.3 HOST MAC ADDRESS LOW REGISTER (HMAC_ADDRL)

typedef union {
    uint32_t value;
    struct {
        unsigned ADDR0:8;
        unsigned ADDR1:8;
        unsigned ADDR2:8;
        unsigned ADDR3:8;
    };
} REG_HMAC_ADDR_L;

#endif