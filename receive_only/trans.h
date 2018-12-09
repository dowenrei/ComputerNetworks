

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define SRC_PORT 21
#define DEST_PORT 21 
//transmit
void transmit_data(uint8_t* data,uint8_t length,uint8_t* transport_packet,uint8_t isframe);

//receive and compare the data, 
void receive(uint8_t* net_packet,uint8_t length);

uint16_t checksum(uint8_t*data,uint8_t length,uint8_t ctrl);

