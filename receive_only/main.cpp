/** \file main.c
 * \author Domenico Balsamo
 * \version 1.0
 * \date 2016/12/10
 */

#pragma GCC diagmostic ignored "-Wwrite-strings"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "trans.h"
#include "rfm12.h"
#include "trans3.cpp"
#include<string.h>

#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 121
#define APP_MAX 114

uint8_t str[APP_MAX];	//the string want to send, taking each word as 1 byte
char dest;	// Address for Network Layer
char c; // read uart char
uint8_t countchar=0; // count char in uart
uint8_t isframe=0;
uint8_t *bufptr;


int main(void)
{
	//testing array 
    //uint8_t app_data[6]={0x67,0x01,0x02,0x05,0x01,0x50};    
	uint8_t transport_packet[TRANS_MAX];
    //printf("Data %d",sizeof(app_data));
	uint8_t hello[7]={0x42,0x41,0x43,0x44,0x45,0x46,0x47};
	uint8_t network_packet[TRANS_MAX] = {0x5d, 0x0b, SRC_PORT, DEST_PORT, 0x06, 0x67, 0x01,0x02,0x05,0x01,0x50,0x40,0x7a};
// If put at rfm status complete, there might be a chance where we dont get to transmit message , so do a while loop for u_art, break loop after 2nd enter
// Get address and store at global variable for network   

	init_uart0();   //Initializa the uart	

	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
    rfm12_init();    //init the RFM12
	_delay_ms(100);
	sei();           //interrupts on
	put_str("rfm12_rx_status()  \n\r");	
	while(1){
		    
	if (rfm12_rx_status() == STATUS_COMPLETE)
		{
                        put_str("rfm12_rx_status() = STATUS_COMPLETE \n\r");	

			bufptr = rfm12_rx_buffer(); //get the address of the current rx buffer

			// dump buffer contents to uart			
			for (uint8_t i=0;i<rfm12_rx_len();i++)
			{
				put_ch(bufptr[i]);
			}
				
				// tell the implementation that the buffer
				// can be reused for the next data.
				rfm12_rx_clear();
				//_delay_ms(1000);
	}

	}

	
}


