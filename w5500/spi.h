

#ifndef _W5500_SPI_H    /* Guard against multiple inclusion */
#define _W5500_SPI_H

void w5500_spi_exchange_buffer(
    NIC* nic, uint8_t* buffer, uint8_t control_byte,
    uint16_t spi_addr, uint16_t length
);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
