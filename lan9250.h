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










typedef struct {
    uint32_t MAC_ADDR_H;
    uint32_t MAC_ADDR_L;
} LAN9250Config;







void lan9250_init(char, LAN9250Config);


#endif