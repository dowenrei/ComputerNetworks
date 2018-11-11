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

#include "rfm12.h"


uint8_t *bufptr;
char str[100];	//the string want to send
uint16_t p = 0;	//the position of string
char c;
uint8_t detect(void);
void sync(void);
uint8_t i=0;

int main(void)
{
	init_uart0();   //Initializa the uart	
	_delay_ms(100);  //little delay for the rfm12 to initialize properly
	
        rfm12_init();    //init the RFM12
	_delay_ms(100);
	sei();           //interrupts on
	
        while(1)
	{
		if ((c = uart0_read()) != -1) {
			put_ch(c);
			str[i] = c;
			i++;	
			if(c == '\n' || c == '\r') {
				put_str("\n\r");
				str[i+1] = '\n';
				put_str(str);
				*str = 0;
				i = 0;
			}	
		}


	}
}


