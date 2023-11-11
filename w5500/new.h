#ifndef _W5500_NEW_H    /* Guard against multiple inclusion */
#define _W5500_NEW_H

#include "../nic/nic.h"

void w5500_init(NIC* nic){
    
}


void w5500_new(NIC* nic){
    nic->init = w5500_init;
}

#endif