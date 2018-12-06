#include "uart.h"


void init_uart0 (void)
{
	/* Configure 9600 baud , 8-bit , no parity and one stop bit */
	const int baud_rate = 9600;
	UBRR0H = (F_CPU/(baud_rate*16L)-1) >> 8;
	UBRR0L = (F_CPU/(baud_rate*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}
int uart0_read(void)
{
	if (!( UCSR0A & _BV(RXC0)))
		return -1;
	return UDR0;
}

char get_ch (void)
{
	while (!( UCSR0A & _BV(RXC0)));
	return UDR0 ;
}
void put_ch ( char ch)
{
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = ch;
}

void put_str (char *str)
{
	int i;
	for (i=0; str[i]; i++) put_ch(str[i]);
}

//Print A Byte in HexaDecimal 
void printByte(uint8_t byte){
	uint8_t first_half=((0xF0 & byte)>>4);
	uint8_t second_half=(0x0F & byte);
	put_ch(toCharInHex(first_half));
	put_ch(toCharInHex(second_half));
}
//Can this be improved?
char toCharInHex(uint8_t h_byte){
switch(h_byte){
	case 0x0:
	return '0';
	case 0x1:
	return '1';
	case 0x2:
	return '2';
	case 0x3:
	return '3';
	case 0x4:
	return '4';
	case 0x5:
	return '5';
	case 0x6:
	return '6';
	case 0x7:
	return '7';	
	case 0x8:
	return '8';	
	case 0x9:
	return '9';
	case 0xA:
	return 'A';
	case 0xB:
	return 'B';
	case 0xC:
	return 'C';
	case 0xD:
	return 'D';
	case 0xE:
	return 'E';
	case 0xF:
	return 'F';	
}
}

