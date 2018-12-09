/** \file main.c
 * \author Domenico Balsamo
 * \version 1.0
 * \date 2016/12/10
 */


#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "rfm12.h"


uint8_t *bufptr;
char str[100];	//the string want to send
uint16_t p = 0;	//the position of string

uint8_t detect(void);
void sync(void);


int main(void)
{
	init_uart0();   //Initializa the uart	
	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
        rfm12_init();    //init the RFM12
	_delay_ms(100);
	
        uint8_t tv[] = "pluto\n\r";
	sei();           //interrupts on
	
        while(1)
	{
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
					
	                                put_str("transmit \n\r");
					put_str(".\n\r");

				rfm12_tx(sizeof(tv), 0, tv); //it doesn't transmit, just buffer
			//	}
				//rfm12 needs to be called from your main loop periodically.
				//it checks if the rf channel is free (no one else transmitting), and then
				//sends packets, that have been queued by rfm12_tx above.
			for (uint8_t j = 0; j < 100; j++)	
			{	
				rfm12_tick();	
				_delay_us(500); 
			}
			_delay_ms(500);	//small delay so loop doesn't run as fast
			
		}
}


