#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lan9250_nic_def.h"
#include "lldp.h"
#include "network.h"
#include "lan9250_crc.h"


void lldp_on_chassis_id(LAN9250Resource *nic, LLDP_TLV *tlv){
    if(LLDP_TYPE_CHASSIS_ID_SUBTYPE_MAC == tlv->information[0]){
        // got MAC address
        uint8_t* source_mac = nic->buffer.bytes + 6;
        uint8_t* chassis_mac = tlv->information + 1;
        if(0 != memcmp(source_mac, chassis_mac, 6)) return;
        memcpy(nic->peer_mac, chassis_mac, 6);
        nic->peer_mac_set = true;
        printf(
            "NIC%d - PeerMAC %x:%x:%x:%x:%x:%x\n",
            nic->id,
            chassis_mac[0], chassis_mac[1], chassis_mac[2],
            chassis_mac[3], chassis_mac[4], chassis_mac[5]
        );
    }
}


bool lldp_parse_ethernet_buffer(LAN9250Resource *nic){
    uint8_t ACCEPTED_DEST_ADDR[6] = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E };
    if(nic->bufferSize < 16) return false;
    
    if(
        nic->buffer.frame.type_or_length_high != 0x88 ||
        nic->buffer.frame.type_or_length_low != 0xCC
    ){
        return false;
    }

    nic->decisions.forward = false;
    nic->decisions.decided = true;
    
    if(!network_compare_mac_addr(nic->buffer.bytes+0, ACCEPTED_DEST_ADDR)){
        return false;
    }
    
    // now we are certain nic->buffer contains a LLDP packet.

    LLDP_PARSER_STATE_MACHINE parser;
    parser.state = READING_CHAR0;
    for(uint16_t i=14; i<nic->bufferSize; i++){
        uint8_t current_byte = nic->buffer.bytes[i];
        switch(parser.state){
            case(READING_CHAR0):
                parser.result.header.char0 = current_byte;
                parser.state = READING_CHAR1;
                break;
            case(READING_CHAR1):
                parser.result.header.char1 = current_byte;
                parser.bufferedSize = 0;
                
                switch(parser.result.header.TYPE){
                    case LLDP_TYPE_CHASSIS_ID: // chassis ID
                    case LLDP_TYPE_PORT_ID: // port ID
                    case LLDP_TYPE_TTL: // TTL
                        parser.state = BUFFERING_BODY;
                        break;
                    case LLDP_TYPE_END: // End
                        parser.state = DONE_WITHOUT_RESULT;
                        break;
                    default:
                        parser.state = BUFFERING_BODY;
                        parser.skipResult = true;
                        break;
                }
                break;
            case(BUFFERING_BODY):
                parser.result.information[parser.bufferedSize] = current_byte;
                parser.bufferedSize += 1;
                if(parser.bufferedSize >= parser.result.header.LENGTH){
                    parser.state = DONE_WITH_RESULT;
                }
                break;
            default:
                break;
        }
        // if result is coming out, do it...
        switch(parser.state){
            case(DONE_WITH_RESULT):
                if(LLDP_TYPE_CHASSIS_ID == parser.result.header.TYPE){
                    lldp_on_chassis_id(nic, &parser.result);
                }
                parser.state = READING_CHAR0;
                break;
            case(DONE_WITHOUT_RESULT):
                parser.state = READING_CHAR0;
            default:
                break;
        }
    }

    return true;
}


// LLDP broadcaster

uint8_t lldp_broadcast_buffer[2][50];
uint8_t lldp_broadcast_buffer_size[2];

uint8_t* lldp_get_broadcast_buffer(LAN9250Resource *nic){
    uint8_t* ret = lldp_broadcast_buffer[nic->id];
    if(lldp_broadcast_buffer_size[nic->id]){
        return ret;
    }
    
    // generates the MAC broadcasting buffer
    
    memcpy(ret, (uint8_t[]){0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e}, 6); // dest
    memcpy(ret+6, nic->local_mac, 6);   // source
    ret[13] = 0x88;
    ret[14] = 0xCC;
    
    ret[15] = 0x02; ret[16] = 0x07;
    ret[17] = 0x04;
    memcpy(ret+17, nic->local_mac, 6);
    
    ret[23] = 0x04; ret[24] = 0x07;
    ret[25] = 0x03;
    memcpy(ret+25, nic->local_mac, 6);
    
    memcpy(ret+31, (uint8_t[]){0x06, 0x02, 0x0e, 0x11}, 4); // TTL
    memcpy(ret+35, (uint8_t[]){0x00, 0x00}, 2); // END
    
    uint32_t crc32 = lan9250_crc32(ret+14, 38-14);
    ret[38] = (crc32 >> 24) & 0xFF;
    ret[39] = (crc32 >> 16) & 0xFF;
    ret[40] = (crc32 >> 8) & 0xFF;
    ret[41] = crc32 & 0xFF;
    
    lldp_broadcast_buffer_size[nic->id] = 42;
    return ret;
}

uint8_t lldp_get_broadcast_buffer_size(LAN9250Resource* nic){
    if(0 == lldp_broadcast_buffer_size[nic->id]){
        lldp_get_broadcast_buffer(nic);
    }
    return lldp_broadcast_buffer_size[nic->id];
}