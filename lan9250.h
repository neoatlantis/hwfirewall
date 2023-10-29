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


















void lan9250_init(void);
void lan9250_run_once(void);


#endif