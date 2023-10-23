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

#ifndef _SPI_H    /* Guard against multiple inclusion */
#define _SPI_H

#include <stdint.h>
#include <stdbool.h>

#define revert_endian(buf) \
    ((buf & 0x000000FF) << 24) |  \
    ((buf & 0x0000FF00) << 8)  |  \
    ((buf & 0x00FF0000) >> 8)  |  \
    ((buf & 0xFF000000) >> 24)

#define SPIxCON_MODE_8      (0b00 << 10)
#define SPIxCON_MODE_16     (0b01 << 10)
#define SPIxCON_MODE_32     (0b11 << 10)

#define SPIxCON_ON          (0b1 << 15)

#define SPIxCON_MSTEN_MASTER (0b1 << 5)
#define SPIxCON_MSTEN_SLAVE  (0b0 << 5)


// DEVICE SPECIFIC COMMANDS
#define SPI_SLOT1_CS_TRISBIT  TRISGbits.TRISG13
#define SPI_SLOT2_CS_TRISBIT  TRISBbits.TRISB1
#define SPI_SDI_TRISBIT       TRISGbits.TRISG7
#define SPI_SDO_TRISBIT       TRISGbits.TRISG8

#define SPI_SLOT1_CS_BIT      LATGbits.LATG13
#define SPI_SLOT2_CS_BIT      LATBbits.LATB1



void spi2_enable();
uint32_t spi2_send_and_receive_new(uint32_t, bool, bool);
uint32_t spi2_receive_and_send_new(uint32_t c, bool);

void spi2_set_mode_8();
void spi2_set_mode_16();
void spi2_set_mode_32();


#endif
