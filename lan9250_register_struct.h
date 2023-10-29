//#include <stdint.h>

#ifndef _LAN9250_REGISTER_STRUCT
#define _LAN9250_REGISTER_STRUCT

// 5.0 FIFOs

#define ADDR_RX_DATA_FIFO 0x00
#define ADDR_TX_DATA_FIFO 0x20
#define ADDR_RX_STATUS_FIFO  0x40
#define ADDR_RX_STATUS_FIFO_PEEK 0x44
#define ADDR_TX_STATUS_FIFO  0X48
#define ADDR_TX_STATUS_FIFO_PEEK 0x4C

// 6.2.3.1 Reset Control Register (RESET_CTL)

typedef union {
    uint32_t value;
    struct {
        unsigned DIGITAL_RST:1;
        unsigned PHY_RST:1;
        unsigned :3;
        unsigned HMAC_RST:1;
        unsigned :26;
    };
} REG_RESET_CTL;
#define ADDR_RESET_CTL 0x1F8

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
#define ADDR_IRQ_CFG 0x54

// 8.3.2 INTERRUPT STATUS REGISTER (INT_STS)

typedef union {
    uint32_t value;
    struct {
        unsigned :3;
        unsigned RSFL:1;        // RX Status FIFO Level Interrupt 
        unsigned RSFF:1;        // RX Status FIFO Full Interrupt
        unsigned :1;
        unsigned RXDF_INT:1;    // RX Dropped Frame Interrupt
        unsigned TSFL:1;        // TX Status FIFO Level Interrupt
        unsigned TSFF:1;        // TX Status FIFO Full Interrupt
        unsigned TDFA:1;        // TX Data FIFO Available Interrupt
        unsigned TDFO:1;        // TX Data FIFO Overrun Interrupt
        unsigned :1;
        unsigned GPIO:1;        // GPIO Interrupt Event
        unsigned TXE_INT:1;     // Transmitter Error Interrupt
        unsigned RXE_INT:1;     // Receiver Error Interrupt
        unsigned RWT_INT:1;     // Receive Watchdog Time-out Interrupt
        unsigned TXSO:1;        // TX Status FIFO Overflow Interrupt
        unsigned PME_INT:1;     // Power Management Event Interrupt
        unsigned :1;
        unsigned GPT_INT:1;     // GP Timer Interrupt
        unsigned RXD_INT:1;     // RX DMA Interrupt
        unsigned TIOC_INT:1;    // TX IOC Interrupt
        unsigned :1;
        unsigned RXDFH_INT:1;   // RX Dropped Frame Counter Halfway Interrupt
        unsigned RXSTOP_INT:1;  // RX Stopped Interrupt
        unsigned TXSTOP_INT:1;  // TX Stopped Interrupt
        unsigned PHY_INT:1;     // PHY Interrupt Event
        unsigned :2;
        unsigned _1588_EVNT:1;  // 1588 Interrupt Event
        unsigned READY:1;       // Device Ready
        unsigned SW_INT:1;      // Software Interrupt
    };
} REG_INT_STS;
#define ADDR_INT_STS 0x58

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
#define ADDR_INT_EN 0x5C

// 17.1 Miscellaneous System Configuration & Status Registers

// 17.1.1 CHIP ID AND REVISION

typedef union {
    uint32_t value;
    struct {
        unsigned REVISION: 16;
        unsigned ID: 16;
    };
} REG_ID_REV;
#define ADDR_ID_REV 0x50

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
#define ADDR_HW_CFG 0x74

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
#define ADDR_FIFO_INT 0x68

// 11.14.2 RECEIVE CONFIGURATION REGISTER (RX_CFG)

typedef union {
    uint32_t value;
    struct{
        unsigned :8;
        unsigned RXDOFF:5;
        unsigned :2;
        unsigned RX_DUMP:1;
        unsigned RX_DMA_CNT:12;
        unsigned :2;
        unsigned RX_EA:2;
    };
} REG_RX_CFG;
#define ADDR_RX_CFG 0x6C

// 11.14.4 RECEIVE DATAPATH CONTROL REGISTER (RX_DP_CTRL)
typedef union {
    uint32_t value;
    struct{
        unsigned :31;
        unsigned RX_FFWD:1;
    };
} REG_RX_DP_CTRL;
#define ADDR_RX_DP_CTRL 0x78


// 11.14.5 RX FIFO INFORMATION REGISTER (RX_FIFO_INF)

typedef union {
    uint32_t value;
    struct {
        unsigned RXDUSED:16;    // bytes of total data in RX FIFO
        unsigned RXSUSED:8;     // dwords of status in RX Status FIFO
        unsigned :8;
    };
} REG_RX_FIFO_INF;
#define ADDR_RX_FIFO_INF 0x7C

// 11.14.6 TX FIFO INFORMATION REGISTER (TX_FIFO_INF)

typedef union {
    uint32_t value;
    struct {
        unsigned TXFREE:16;
        unsigned TXSUSED:8;
        unsigned :8;
    };
} REG_TX_FIFO_INF;
#define ADDR_TX_FIFO_INF 0x80

// 11.14.7 HOST MAC RX DROPPED FRAMES COUNTER REGISTER (RX_DROP)

typedef union {
    uint32_t value;
    struct {
        unsigned RX_DFC:32;
    };
} REG_RX_DROP;
#define ADDR_RX_DROP 0xA0

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
#define ADDR_MAC_CSR_CMD 0xA4
#define ADDR_MAC_CSR_DATA 0xA8


// 11.15.1 HOST MAC CONTROL REGISTER (HMAC_CR)

typedef union {
    uint32_t value;
    struct {
        unsigned :2;
        unsigned RXEN:1;
        unsigned TXEN:1;
        unsigned :1;
        unsigned DFCHK:1;
        unsigned BOLMT:2;
        unsigned PADSTR:1;
        unsigned :1;
        unsigned DISRTY:1;
        unsigned BCAST:1;
        unsigned :1;
        unsigned HPFILT:1;
        unsigned :1;
        unsigned HO:1;
        unsigned PASSBAD:1;
        unsigned INVFILT:1;
        unsigned PRMS:1;
        unsigned MCPAS:1;
        unsigned FDPX:1;
        unsigned LOOPPBK:1;
        unsigned :1;
        unsigned PCVOWN:1;
        unsigned :1;
        unsigned HMAC_EEE_ENABLE:1;
        unsigned :5;
        unsigned RXALL:1;
    };
} REG_HMAC_CR;



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




///////////////////////////////////////////////////////////////////////////////
// We here define dwords that are retrieved out of FIFO, which do not belong
// to any registers.

// 11.12.3 RX STATUS FORMAT

typedef union {
    uint32_t value;
    struct {
        unsigned :1;
        unsigned CRC_ERROR:1;
        unsigned DRIBBLING_BIT:1;
        unsigned MII_ERROR:1;
        unsigned RX_WDT_ERROR:1;
        unsigned FRAME_TYPE:1;
        unsigned COLLISION_SEEN:1;
        unsigned FRAME_TOO_LONG:1;
        unsigned :2;
        unsigned MULTICAST_FRAME:1;
        unsigned RUNT_FRAME:1;
        unsigned LENGTH_ERROR:1;
        unsigned BROADCAST_FRAME:1;
        unsigned :1;
        unsigned ERROR_STATUS:1;
        unsigned LENGTH:14;
        unsigned FILTERING_FAIL:1;
        unsigned PACKET_FILTER:1;
    };
} DWORD_RX_STATUS;





#endif