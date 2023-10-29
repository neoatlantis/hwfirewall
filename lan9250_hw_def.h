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

#ifndef _LAN9250_HW_DEF_H    /* Guard against multiple inclusion */
#define _LAN9250_HW_DEF_H

void lan9250_select_1(void);
void lan9250_select_2(void);
void lan9250_deselect_1(void);
void lan9250_deselect_2(void);
void lan9250_reset_1(void);
void lan9250_reset_2(void);
void lan9250_enable_interrupt_1(void);
void lan9250_enable_interrupt_2(void);
void lan9250_disable_interrupt_1(void);
void lan9250_disable_interrupt_2(void);
void lan9250_clear_interrupt_1(void);
void lan9250_clear_interrupt_2(void);


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
