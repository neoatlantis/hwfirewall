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

uint32_t lan9250_read_dword(LAN9250Resource nic, uint16_t addr);
bool lan9250_write_dword(LAN9250Resource nic, uint16_t addr, uint32_t value);
bool lan9250_read_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n);
bool lan9250_write_n_bytes(LAN9250Resource nic, uint32_t* buffer, size_t bufferSize, uint16_t addr, size_t n);




#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
