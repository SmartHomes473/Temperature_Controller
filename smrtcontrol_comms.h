/*
 * This header provides the interface for serializing/deserializing the Roomba function calls
 */
#include <stdint.h>

/* '0' is ASCII is 48 */
#define ASCII_OFFSET 48

/* SMRTControl Packet Delimiters */
#define PACKET_START_BYTE 0x0F
#define PACKET_END_BYTE 0x04


/*
 * Register Temp device on SMRTControl
 */
void register_device(uint8_t* device_num_addr);

/*
 * Parse incoming packet
 */
uint8_t* parse_packet();

