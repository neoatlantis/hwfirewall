#ifndef _LAN9250_NIC_DEF_H
#define _LAN9250_NIC_DEF_H

#include <stdbool.h>
#include "lan9250_register_struct.h"
#include "lan9250_config.h"
typedef struct {
    REG_RESET_CTL       RESET_CTL;
    REG_IRQ_CFG         IRQ_CFG;
    REG_INT_STS         INT_STS;        // updated on interrupt, reflects the last interrupt status
    REG_INT_EN          INT_EN;
    REG_ID_REV          ID_REV;
    REG_HW_CFG          HW_CFG;
    REG_FIFO_INT        FIFO_INT;
    REG_RX_CFG          RX_CFG;
    REG_TX_CFG          TX_CFG;
    REG_RX_DP_CTRL      RX_DP_CTRL;
    REG_RX_FIFO_INF     RX_FIFO_INF;
    REG_TX_FIFO_INF     TX_FIFO_INF;
    REG_RX_DROP         RX_DROP;
    REG_MAC_CSR_CMD     MAC_CSR_CMD;
    REG_HMAC_ADDR_H     HMAC_ADDR_H;
    REG_HMAC_ADDR_L     HMAC_ADDR_L;
    REG_HMAC_CR         HMAC_CR;
} LAN9250Registers;

struct LAN9250EthernetFrame {
    uint8_t destination[6];
    uint8_t source[6];
    uint8_t type_or_length_high;
    uint8_t type_or_length_low;
    uint8_t payload[LAN9250_NIC_BUFFER_SIZE-12-2];
};

typedef union {
    uint8_t bytes[sizeof(struct LAN9250EthernetFrame)];
    struct LAN9250EthernetFrame frame;
} LAN9250EthernetPacket;

struct LAN9250EthernetPacketDecisions {
    bool decided;

    bool forward;
};


typedef struct {
    uint8_t id;
    LAN9250EthernetPacket buffer;
    uint16_t bufferSize;
    uint8_t peer_mac[6];
    bool peer_mac_set;
    struct LAN9250EthernetPacketDecisions decisions;
    void (*select)(void);
    void (*deselect)(void);
    void (*hardware_reset)(void);
    void (*enable_interrupt)(void);
    void (*disable_interrupt)(void);
    void (*clear_interrupt)(void);
    LAN9250Registers registers;
} LAN9250Resource;


#endif