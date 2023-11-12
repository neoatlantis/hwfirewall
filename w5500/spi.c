#include <stddef.h>
#include "../nic/nic.h"

void w5500_spi_exchange_buffer(
    NIC* nic, uint8_t* buffer, uint8_t control_byte,
    uint16_t spi_addr, uint16_t length
){
    uint8_t leading_bytes[3];
    leading_bytes[0] = (spi_addr >> 8) & 0xFF;
    leading_bytes[1] = spi_addr & 0xFF;
    leading_bytes[2] = control_byte;
    if(NULL != nic->driver.spi_select) nic->driver.spi_select();
    nic->driver.spi_exchange_register(leading_bytes, 3);
    nic->driver.spi_exchange_register(buffer, length);
    if(NULL != nic->driver.spi_deselect) nic->driver.spi_deselect();
}