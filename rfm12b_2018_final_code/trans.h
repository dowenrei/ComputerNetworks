

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include "./trans3.cpp"
//transmit
void transmit_data(uint8_t* data,uint8_t length,uint8_t* transport_packet);

//receive and compare the data, 
void receive(uint8_t* net_packet,uint8_t length);

uint16_t checksum(uint8_t*data,uint8_t length);

