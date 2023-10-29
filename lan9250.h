/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _LAN9250_H    /* Guard against multiple inclusion */
#define _LAN9250_H


#include "spi.h"
#include <xc.h>


#include "lan9250_register_struct.h"
typedef struct {
    REG_RESET_CTL       RESET_CTL;
    REG_IRQ_CFG         IRQ_CFG;
    REG_INT_STS         INT_STS;
    REG_INT_EN          INT_EN;
    REG_ID_REV          ID_REV;
    REG_HW_CFG          HW_CFG;
    REG_FIFO_INT        FIFO_INT;
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







typedef struct {
    uint32_t MAC_ADDR_H;
    uint32_t MAC_ADDR_L;
} LAN9250Config;


// to write a register via SPI, use example:
//  lan9250_write_sysreg(IRQ_CFG);
// and translates to
//  lan9250_write_dword(nic, ADDR_IRQ_CFG, &nic->registers.IRQ_CFG.value);

#define __NIC_REGISTERS_X_VALUE(name) nic->registers.name.value
#define __ADDR_X(name)                ADDR_ ## name
#define lan9250_write_sysreg(name) \
    lan9250_write_dword(nic, __ADDR_X(name), &(__NIC_REGISTERS_X_VALUE(name)))
#define lan9250_read_sysreg(name) \
    lan9250_read_dword(nic, __ADDR_X(name), &(__NIC_REGISTERS_X_VALUE(name)))




void lan9250_init(char, LAN9250Config);


#endif