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

#ifndef _UART_H    /* Guard against multiple inclusion */
#define _UART_H

#include "system_config.h"
#define BAUDRATE 115200
#define BRG_VAL  (PBCLK / 16 / BAUDRATE - 1)

void putch(char);
void uart2_enable();

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
