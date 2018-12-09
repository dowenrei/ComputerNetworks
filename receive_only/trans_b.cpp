// Use this
//#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

//memcpy need this
#include<string.h>

//#include <util/delay.h>
//#include <avr/interrupt.h>
//#include "trans.h"
#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 114

uint8_t seq=0x00;

struct trans{
    uint8_t ctrl[2];
    uint8_t src;
    uint8_t dest;
    uint8_t length;
    uint16_t checksum; //need?
    //data?
};

// can add case 
uint16_t checksum(uint8_t* data,uint8_t length,uint8_t ctrl){  
    //printf("Length of Checksum_Value: %d",length);
    uint16_t parity=0x0000;
    for(int i=0;i<length;i++){
        //printf("%i",i);
        if (i%2==0){
            parity^=(uint16_t)data[i]<<8;
            //printf("msb %x\n",parity);
        }    
        else {         
            parity^=data[i];
            //printf("lsb %x\n",parity);           
        }
    }
    return parity;
}

//not the correct function but still trying, assume data is word to be transmitted
void transmit(uint8_t* data,uint8_t length,uint8_t* transport_packet){

    struct trans msg;
    uint8_t *t_ptr=transport_packet;
    //trans* msg=(trans*) data;
    uint8_t n;
    //do control
    msg.ctrl[0]=0x00;
    msg.ctrl[1]=seq << 4;
    msg.src=SRC_PORT;
    msg.dest=DEST_PORT;
    msg.length=length;
    n=length+7;

    //concatenate array for checksum, to checksum array varies according to the app_data size 
    uint8_t to_checksum[n];
    to_checksum[0]=msg.ctrl[0];
    to_checksum[1]=msg.ctrl[1];
    to_checksum[2]=msg.src;
    to_checksum[3]=msg.dest;
    to_checksum[4]=msg.length;
    to_checksum[msg.length+5]=0x00;
    to_checksum[msg.length+6]=0x00;
    memcpy(to_checksum+5,data,msg.length);

    for(int i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
    }
    msg.checksum=checksum(to_checksum,sizeof(to_checksum),0x00);   
    printf("\nChecksum %x\n",msg.checksum);
    // MSB, LSB [masking to split msg.checksum to two 8 bits]
    uint8_t checksums[2]={ (uint8_t)(msg.checksum >> 8),(uint8_t)(msg.checksum & 0xFF)};
    
    //to_checksum is now transport_packet after adding the checksum bits
    memcpy(to_checksum+msg.length+5,checksums,sizeof(checksums));
 
     for(int i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
        *(t_ptr+i)=to_checksum[i];

    }
    printf("\n\r ");
    seq+=1;
    if (seq==16){
        seq=0;
    }
}


void receive(uint8_t* net_packet,uint8_t length){

    //currently didn't use length yet 
    trans* net=(trans*) net_packet;
    //Check if the communication is reliable 0b0001 0000
    //printf("Control0 %x ",net->ctrl[0] & (1<<4));
    //If the communication is reliable, set ack bit to 1 and TRANSMIT whole frame?
    if (net->ctrl[0] & (1<<4) == 0x10)
        net->ctrl[0] |= 1<<7;
    

    printf("Control0 %x ",net->ctrl[0] & 0x60 );
    //printf("Received %x huh",*(net_packet+1)); works
    printf("Length %x ",net->length);
    //First Data is always net_packet+5
    printf("First Data %x ",*(net_packet+5));
    //calculate data checksum 
    uint8_t app_data[net->length+5];
    for(int i =0; i<sizeof(app_data);i++){
        app_data[i]=*(net_packet+i);
        //printf("App Data %x ",app_data[i]);
    }

    // struct' checksum = calculated checksum 
    net->checksum=checksum(app_data,sizeof(app_data),net->ctrl[0] & 0x60);
    printf("Calculated Checksum %x",net->checksum);
    //given_checksum
    uint16_t given_checksum =((uint16_t)*(net_packet+5+net->length)<<8)|*(net_packet+5+net->length+1);
    //compare struct' checksum and given then do what?
    printf("Received %x Checksum",given_checksum); 
    //app data 
    for(int i =5; i<sizeof(app_data);i++){
        printf("App Data %x ",app_data[i]);
    }
}



int main(){
    uint8_t app_data[6]={0x61,0x0D,0x00,0x00,0x00,0x00};
    // to network layer    
    uint8_t transport_packet[TRANS_MAX] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00};
    // from network layer, len=3,
    uint8_t network_packet[TRANS_MAX] = {0x5d, 0x0b, SRC_PORT, DEST_PORT, 0x06, 0x67, 0x01,0x02,0x05,0x01,0x50,0x40,0x7a};

 


    
    receive(network_packet,sizeof(network_packet));

    /*
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet); 
    transmit(app_data,sizeof(app_data),transport_packet);
    */
} 


  
