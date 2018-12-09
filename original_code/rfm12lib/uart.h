#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 12000000

//uart
void init_uart0 (void);
char get_ch (void);
void put_ch (char ch);
void put_str (char *str);
int uart0_read(void);
void printByte(uint8_t byte);
char toCharInHex(uint8_t h_byte);
void put_strHex (uint8_t *str,uint8_t len);
