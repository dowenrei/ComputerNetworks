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
#include "trans.h"
#include<string.h>

uint8_t *bufptr;
uint8_t str[114];	//the string want to send, taking each word as 1 byte
uint8_t str_2[2];	//the string want to send, taking each word as 1 byte
uint16_t p = 0;	//the position of string
char c;
uint8_t detect(void);
void sync(void);
uint8_t i=0;

#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 114


int main(void)
{
    uint8_t app_data[6]={0x67,0x01,0x02,0x05,0x01,0x50};    
	uint8_t transport_packet[TRANS_MAX] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00};
    //printf("Data %d",sizeof(app_data));

	put_str("Welcome to A2 Il-Messenger\n\r");	

// If put at rfm status complete, there might be a chance where we dont get to transmit message , so do a while loop for u_art, break loop after 2nd enter
// Get address and store at global variable for network   
	
	init_uart0();   //Initializa the uart	

	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
    rfm12_init();    //init the RFM12
	_delay_ms(100);
	sei();           //interrupts on
	
	transmit_data(app_data,sizeof(app_data),transport_packet);
    while(1)
	{

		//If something is received through uart 
		if ((c = uart0_read()) != -1) {
			put_ch(c);
			str[i] = c;
			i++;	
			//If newline :
			if(c == '\n' || c == '\r') {
				put_str("\n\r");
				str[i+1] = '\n';
				*str = 0;
				i = 0;
				break;
			}
		}
	}
			put_str("Do you still want to talk to this device? [Y,N]\n\r");	

	while(1)
	{

		//If something is received through uart 
		if ((c = uart0_read()) != -1) {
			put_ch(c);
			str_2[i] = c;
			printByte(str_2[i]);
							if(str_2[i]==0x59){
					put_str("hi");
				}
			if(c == '\n' || c == '\r') {
				put_str("\n\r");

			}

			//If newline :

		}
	}	
				
	
}


