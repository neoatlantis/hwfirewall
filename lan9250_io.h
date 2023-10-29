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

#ifndef _LAN9250_IO_H    /* Guard against multiple inclusion */
#define _LAN9250_IO_H

#include "lan9250_nic_def.h"
#include "lan9250_register_struct.h"

bool lan9250_read_dword(LAN9250Resource* nic, uint16_t addr, uint32_t* ret);
bool lan9250_write_dword(LAN9250Resource* nic, uint16_t addr, uint32_t* value);
bool lan9250_read_mac_csr(LAN9250Resource* nic, uint8_t addr, uint32_t *result);
bool lan9250_write_mac_csr(LAN9250Resource* nic, uint8_t addr, uint32_t *value);

/*bool lan9250_read_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n);
bool lan9250_write_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n);*/

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


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
