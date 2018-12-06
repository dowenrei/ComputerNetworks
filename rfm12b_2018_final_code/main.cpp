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

#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 121
#define APP_MAX 114

uint8_t *bufptr;
uint8_t str[APP_MAX];	//the string want to send, taking each word as 1 byte
uint8_t add;	// Address for Network Layer
uint16_t p = 0;	//the position of string
char c;
uint8_t detect(void);
void sync(void);
uint8_t i=0;




int main(void)
{
	//testing array 
    //uint8_t app_data[6]={0x67,0x01,0x02,0x05,0x01,0x50};    
	uint8_t transport_packet[TRANS_MAX] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00};
    //printf("Data %d",sizeof(app_data));

// If put at rfm status complete, there might be a chance where we dont get to transmit message , so do a while loop for u_art, break loop after 2nd enter
// Get address and store at global variable for network   
	
	init_uart0();   //Initializa the uart	

	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
    rfm12_init();    //init the RFM12
	_delay_ms(100);
	sei();           //interrupts on
	put_str("Welcome to A2 Il-Messenger\n\r");

	ADDRESS :
	put_str("Which Device Do You Want to Talk To? \n\r");
	    while(1)
	{
		//Get Destination Address 
		if ((c = uart0_read()) != -1) {
			//put_ch(c);
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
			i++;	
			//If newline :
			if(c == '\n' || c == '\r') {
				put_str("\n\r");
				transmit_data(str,i,transport_packet);
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


