
//memcpy need this
#include<string.h>
#define SRC_PORT 21
#define DEST_PORT 21 
#define APP_MAX 114
#define DEBUG 1
#define RELIABLE 1
#define ADDBITERROR 0
#define TRANS_MAX 121



//transmit
void transmit_data(uint8_t* data,uint8_t length,uint8_t* transport_packet,uint8_t isframe);
//receive and compare the data, 
void receive(uint8_t* net_packet,uint8_t length);
uint16_t checksum(uint8_t*data,uint8_t length,uint8_t ctrl);
void sendAcknowledgement(uint8_t * trans_header);