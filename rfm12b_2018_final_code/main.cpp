/** \file main.c
 * \author Domenico Balsamo
 * \version 1.0
 * \date 2016/12/10
 */

//#pragma GCC diagmostic ignored "-Wwrite-strings"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "rfm12.h"
#include "trans3.cpp"


#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 121
#define APP_MAX 114

uint8_t str[APP_MAX];	//the string want to send, taking each word as 1 byte
uint8_t str_2[APP_MAX]; //for fragmentation
char dest;	// Address for Network Layer
char c; // read uart char
uint8_t countchar=0; // count char in uart
uint8_t isframe=0;
uint8_t *bufptr;


//uint8_t network_packet[TRANS_MAX] = {0x5d, 0x0b, SRC_PORT, DEST_PORT, 0x06, 0x67, 0x01,0x02,0x05,0x01,0x50,0x40,0x7a};
int main(void)
{
 
	uint8_t transport_packet[TRANS_MAX];
 

	init_uart0();   //Initializa the uart	
	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
    rfm12_init();    //init the RFM12
	_delay_ms(100);
	sei();           //interrupts on
	put_str("Welcome to A2 Il-Messenger\n\r");
	while(1){

	ADDRESS :
	put_str("Which Device Do You Want to Talk To? \n\r");
	    while(1)
	{
		//Get Destination Address 
		if ((c = uart0_read()) != -1) {
			dest = c;
			put_ch((char) dest);	
			put_str("\n\r");
			#if DEBUG
			put_str("This address is for Network Layer\n\r");
			#endif
			break;
		}
	}

	MESSAGE : 
	put_str("Type In Your Message \n\r");
    while(1)
	{
		//If something is received through uart 
		if ((c = uart0_read()) != -1) {
			str[countchar] = c;
			put_ch((char) str[countchar]);
			//count characters
			countchar++;
			//Segment Message if Message >114 characters
			if (countchar==APP_MAX-1){
				#if DEBUG
				put_str("\n\r Fragmenting Message \n\r");
				#endif
				isframe+=1;
				if(isframe==16)
        			isframe=0;
				memcpy(str_2,str,sizeof(str));		
				*str = 0;
				countchar = 0;
			_delay_ms(500);	//small delay so loop doesn't run as fast	
			}
			//If newline :
			if(c == '\n' || c == '\r') {
				put_str("\n\r");					
				// Transmit earlier segmented message first 
				if (isframe>=1){
					_delay_us(500); 
					transmit_data(str_2,APP_MAX-1,transport_packet,isframe-1);
					rfm12_tx(sizeof(str), 0, str); //it doesn't transmit, just buffer
					for (uint8_t j = 0; j < 100; j++)	
					{	
					rfm12_tick();	
					_delay_us(500); 
					}
					transmit_data(str,countchar,transport_packet,isframe);
				}
				else{
					transmit_data(str,countchar,transport_packet,isframe);	
				}	
				// Send Message if New Line
				rfm12_tx(sizeof(str), 0, str); //it doesn't transmit, just buffer
				for (uint8_t j = 0; j < 100; j++)	
				{	
				rfm12_tick();	
				_delay_us(500); 
				}		
				_delay_ms(500); 
				*str = 0;
				countchar = 0;	
				isframe=0;
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
				goto MESSAGE; //MESSAGE
				break;
			}
			else if (c == 'N' || c == 'No' || 'n'){
				put_str("\n\r");
				goto RECEIVE; 
				break;
			}
			else{
				put_str("\n\r Please type either Y or N");
				goto NEXT;
				break;
			}
		}
	}
	RECEIVE:	
	receive(transport_packet,sizeof(transport_packet));
	if (rfm12_rx_status() == STATUS_COMPLETE)
		{
            put_str("rfm12_rx_status() = STATUS_COMPLETE \n\r");	
			bufptr = rfm12_rx_buffer(); //get the address of the current rx buffer
			// dump buffer contents to uart			
			for (uint8_t i=0;i<rfm12_rx_len();i++)
			{
				put_ch(bufptr[i]);
			}				
				// tell the implementation that the buffer can be reused for the next data.
				rfm12_rx_clear();
				//_delay_ms(1000);
	}
	goto ADDRESS;

	}

	
}


