typedef union {
    struct {
        uint8_t char1; // second byte being sent from frame, lower
        uint8_t char0; // first byte being sent from frame, higher
    };
    struct {
        unsigned LENGTH:9;
        unsigned TYPE:7;
    };
} LLDP_TLV_HEADER;

#define lldp_is_end(x)         (0==x.TYPE && 0==x.LENGTH)
#define lldp_is_chassis_id(x)  (1==x.TYPE)
#define lldp_is_port_id(x)     (2==x.TYPE)
#define lldp_is_ttl(x)         (3==x.TYPE && 2==x.LENGTH)

typedef struct {
    LLDP_TLV_HEADER header;
    uint8_t information[512];
} LLDP_TLV;


typedef struct {
    uint32_t MAC_ADDR_L;
    uint32_t MAC_ADDR_H;
    uint16_t TTL;
} LLDP_INFO_MAC;


typedef enum {
    READING_CHAR0,
    READING_CHAR1,
    BUFFERING_BODY,
    DONE_WITH_RESULT,
    DONE_WITHOUT_RESULT
} LLDP_PARSER_STATE;

typedef struct {
    LLDP_PARSER_STATE state;
    LLDP_TLV          result;
    uint16_t          bufferedSize;
    bool              skipResult;
} LLDP_PARSER_STATE_MACHINE;

bool lldp_parse_ethernet_buffer(LAN9250Resource *nic);
uint8_t lldp_get_broadcast_buffer_size(LAN9250Resource* nic);
uint8_t* lldp_get_broadcast_buffer(LAN9250Resource *nic);

#define LLDP_TYPE_CHASSIS_ID 1
#define LLDP_TYPE_PORT_ID    2
#define LLDP_TYPE_TTL        3
#define LLDP_TYPE_END        0

#define LLDP_TYPE_CHASSIS_ID_SUBTYPE_MAC 4