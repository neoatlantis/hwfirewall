#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "lan9250_io.h"
#include "lan9250_op.h"
#include "lan9250_nic_def.h"

bool lan9250_test_byte(LAN9250Resource* nic){
    uint32_t value;
    lan9250_read_dword(nic, 0x64, &value);
    return 0x87654321 == value;
}

bool lan9250_read_chip_id(LAN9250Resource* nic){
    lan9250_read_sysreg(ID_REV);
    return 0x9250 == nic->registers.ID_REV.ID;
}

bool lan9250_read_hw_cfg(LAN9250Resource* nic){
    return lan9250_read_sysreg(HW_CFG);
}

bool lan9250_write_hw_cfg(LAN9250Resource* nic){
    nic->registers.HW_CFG.MUST_BE_0 = 0b0;
    nic->registers.HW_CFG.MUST_BE_00 = 0b00;
    nic->registers.HW_CFG.MUST_BE_1 = 0b1;
    return lan9250_write_dword(nic, 0x74, &nic->registers.HW_CFG.value);
}

void lan9250_drop_packet(LAN9250Resource *nic, size_t bytesLength){
    size_t i = 0;
    if(bytesLength >= 16){
        // use fast forward
        nic->registers.RX_DP_CTRL.RX_FFWD = 1;
        lan9250_write_sysreg(RX_DP_CTRL);
        do{
            lan9250_read_sysreg(RX_DP_CTRL);
        } while (nic->registers.RX_DP_CTRL.RX_FFWD);
    } else {
        for(i = 0; i < bytesLength; i += 4){
            //Perform standard PIO read operation
            //lan9250_read_sysreg(RX_DATA_FIFO);
       }
    }
}
