typedef union {
    uint16_t value;
    struct {
        unsigned LENGTH:9;
        unsigned TYPE:7; // lower
    }
} LLDP_TLV_HEADER;

#define lldp_is_end(x)         (0==x.TYPE && 0==x.LENGTH)
#define lldp_is_chassis_id(x)  (1==x.TYPE)
#define lldp_is_port_id(x)     (2==x.TYPE)
#define lldp_is_ttl(x)         (3==x.TYPE && 2==x.LENGTH)

typedef struct {
    LLADP_TLV_HEADER header;
    uint8_t information[512];
} LLDP_TLV;

typedef union {

}