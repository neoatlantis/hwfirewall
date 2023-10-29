#ifndef _LAN9250_NIC_DEF_H
#define _LAN9250_NIC_DEF_H

#include "lan9250_register_struct.h"
typedef struct {
    REG_RESET_CTL       RESET_CTL;
    REG_IRQ_CFG         IRQ_CFG;
    REG_INT_STS         INT_STS;
    REG_INT_EN          INT_EN;
    REG_ID_REV          ID_REV;
    REG_HW_CFG          HW_CFG;
    REG_FIFO_INT        FIFO_INT;
    REG_RX_DP_CTRL      RX_DP_CTRL;
    REG_RX_FIFO_INF     RX_FIFO_INF;
    REG_TX_FIFO_INF     TX_FIFO_INF;
    REG_RX_DROP         RX_DROP;
    REG_MAC_CSR_CMD     MAC_CSR_CMD;
    REG_HMAC_ADDR_H     HMAC_ADDR_H;
    REG_HMAC_ADDR_L     HMAC_ADDR_L;
    REG_HMAC_CR         HMAC_CR;
} LAN9250Registers;

typedef struct {
    uint8_t buffer[1536];
    void (*select)(void);
    void (*deselect)(void);
    void (*hardware_reset)(void);
    void (*enable_interrupt)(void);
    void (*disable_interrupt)(void);
    void (*clear_interrupt)(void);
    LAN9250Registers registers;
} LAN9250Resource;


#endif