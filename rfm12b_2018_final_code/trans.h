

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include "./trans.cpp"
//transmit
uint8_t transmit(uint8_t length,uint8_t* data,uint8_t dest);

//receive and compare the data, 
uint8_t receive(uint8_t* data);

