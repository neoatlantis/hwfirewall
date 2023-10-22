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
    uint8_t buffer[1536];
    void (*select)(void);
    void (*deselect)(void);
} LAN9250Resource;

void lan9250_select_1(void);
void lan9250_select_2(void);
void lan9250_deselect_1(void);
void lan9250_deselect_2(void);






void lan9250_init(char);


#endif