/** \file main.c
  A2 - Network & App 
 */



#pragma GCC diagmostic ignored "-Wwrite-strings"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "rfm12.h"
#include "trans.h"
#include<string.h>

#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 121
#define APP_MAX 114

uint8_t str[APP_MAX];	//the string want to send, taking each word as 1 byte
uint8_t add;	// Address for Network Layer
uint16_t p = 0;	//the position of string
char c; // read uart char
uint8_t i=0; // count char in uart
uint8_t isframe=0; // Segment the word if > 114 bytes



int main(void)
{
	// to network layer
	uint8_t transport_packet[TRANS_MAX];
	// from network layer
	uint8_t network_packet[TRANS_MAX] = {0x5d, 0x0b, SRC_PORT, DEST_PORT, 0x06, 0x67, 0x01,0x02,0x05,0x01,0x50,0x40,0x7a};
// If put at rfm status complete, there might be a chance where we dont get to transmit message , so do a while loop for u_art, break loop after 2nd enter

	init_uart0();   //Initializa the uart	

	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
    rfm12_init();    //init the RFM12
	_delay_ms(100);
	sei();           //interrupts on
	put_str("Welcome to A2 Il-Messenger\n\r");
	put_strHex(hello,sizeof(hello));
	ADDRESS :
	put_str("Which Device Do You Want to Talk To? \n\r");
	    while(1)
	{
		//Get Destination Address 
		if ((c = uart0_read()) != -1) {
			add = c;
			put_ch((char) add);	
			put_str("\n\r");
			break;
		}
	}

	MESSAGE : 
	put_str("Type In Your Message \n\r");
    while(1)
	{
		//If something is received through uart 
		if ((c = uart0_read()) != -1) {
			//put_ch(c);
			str[i] = c;
			put_ch((char) str[i]);
			//count characters
			i++;
			//Segment Message if Message >114 characters
			if (i==113){
				isframe+=1;
				if(isframe==16)
        			isframe=0;
				transmit_data(str,i,transport_packet,isframe);
				*str = 0;
				i = 0;
			}
			//If newline :
			if(c == '\n' || c == '\r') {
				put_str("\n\r");
				isframe=0;
				transmit_data(str,i,transport_packet,isframe);
				/*
				//Print Transport Packet
				for(uint8_t a =0; a<i+7;a++){
        			printByte(transport_packet[a]);
        			put_ch(' ');
    			}
				*/
				*str = 0;
				i = 0;	
				break;
			} 
		}
	}
	NEXT:
	put_str("\n\rDo you still want to talk to this device? [Y,N]\n\r");	
    while(1)
	{
		//If something is received through uart 
		if ((c = uart0_read()) != -1) {
			put_ch(c);	

			if(c == 'Y' || c == 'Yes' ||c == 'y' ) {
				put_str("\n\r");
				goto MESSAGE;
				break;
			}
			else if (c == 'N' || c == 'No' || 'n'){
				put_str("\n\r");
				goto ADDRESS;
				break;
			}
			else{
				put_str("\n\r Please type either Y or N");
				goto NEXT;
				break;
			}
		}
	}
	
}


