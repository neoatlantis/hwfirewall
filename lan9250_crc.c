#include <stdint.h>
#include <stddef.h>

uint32_t lan9250_crc32(const void *data, uint16_t length){
    uint16_t i;
    uint16_t j;
    uint32_t crc;
    const uint8_t *p;
  
    //Point to the data over which to calculate the CRC
    p = (uint8_t *) data;
    //CRC preset value
    crc = 0xFFFFFFFF;
  
    //Loop through data
    for(i = 0; i < length; i++){
       //The message is processed bit by bit
       for(j = 0; j < 8; j++){
          //Update CRC value
          if((((crc >> 31) ^ (p[i] >> j)) & 0x01) != 0){
              crc = (crc << 1) ^ 0x04C11DB7;
          } else {
              crc = crc << 1;
          }
       }
    }
  
    //Return CRC value
    return crc;
}