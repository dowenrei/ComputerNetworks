#include "rfm12.h"
#include "trans.h"

#include <stdio.h>
#include <stdint.h>
//memcpy need this
#include<string.h>
#define SRC_PORT 21
#define DEST_PORT 21 


uint8_t seq=0x00;
struct trans{
    uint8_t ctrl[2];
    uint8_t src;
    uint8_t dest;
    uint8_t length;
    uint16_t checksum; //need?
    //data?
};

// Ctrl : 0x60 Fletcher , 0x40 CRC, 0x20 Internet Checksum 
uint16_t checksum(uint8_t* data,uint8_t length,uint8_t ctrl){
    printf("In Checksum %x",ctrl);
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
void transmit_data(uint8_t* data,uint8_t length,uint8_t* transport_packet,uint8_t isframe){
    //printByteprintByte(isframe);
    struct trans msg;
    uint8_t *t_ptr=transport_packet;
    //trans* msg=(trans*) data;
    uint8_t n;

    //do control 
    msg.ctrl[0]=0x00; //[0Message,00Checksum,0Unreliable,0NoFragment,0Nth,00]
    if(isframe !=0x00)
        msg.ctrl[0] |= 1<<3;    
    msg.ctrl[1]=seq<<4 | isframe; //[Sequence -4bits, Fragment 4 bits]
    msg.src=SRC_PORT;

    //put_ch((char) SRC_PORT);
    msg.dest=DEST_PORT;

    msg.length=length;
    //cannot create variable size length, assume message is 6
    n=msg.length+7;

    //concatenate array for checksum, to checksum array varies according to the header_data size 
    uint8_t to_checksum[n];
    to_checksum[0]=msg.ctrl[0];
    to_checksum[1]=msg.ctrl[1];
    to_checksum[2]=msg.src;
    to_checksum[3]=msg.dest;
    to_checksum[4]=msg.length;
    to_checksum[msg.length+5]=0x00;
    to_checksum[msg.length+6]=0x00;
    //{msg.ctrl[0],msg.ctrl[1],msg.src,msg.dest,msg.length};

    //concatenate header with data 
    memcpy(to_checksum+5,data,msg.length);

    for(uint8_t i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
        //printByte(to_checksum[i]);
        //put_ch(' ');
    }
    put_str("\n\rAfter Checksum \n\r");


    msg.checksum=checksum(to_checksum,sizeof(to_checksum),0x00);   
    printf("\nChecksum %x\n",msg.checksum);
    // MSB, LSB [masking to split msg.checksum to two 8 bits]
    uint8_t checksums[2]={ (uint8_t)(msg.checksum >> 8),(uint8_t)(msg.checksum & 0xFF)};
    
    //to_checksum is now transport_packet after adding the checksum bits
    memcpy(to_checksum+msg.length+5,checksums,sizeof(checksums));
 
     for(uint8_t i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
        *(t_ptr+i)=to_checksum[i];
        printByte(to_checksum[i]);
        //put_ch((char)to_checksum[i]);
        put_ch(' ');
    }
    put_str("\n\r");
    //Increase seq number if it's not framed
    if(isframe==0)
        seq+=1;
    if (seq==16)
        seq=0;

}


void receive(uint8_t* net_packet,uint8_t length){
    //currently didn't use length yet 
    trans* net=(trans*) net_packet;

    //If the communication is reliable, set ack bit to 1 and TRANSMIT whole frame?
    if (net->ctrl[0] & (1<<4) == 0x10)
        net->ctrl[0] |= 1<<7;

    //printByte(net->src);
    //printByte(net->dest);
    printf("Length %x ",net->length);
    //First Data is always net_packet+5
    printf("First Data %x ",*(net_packet+5));
    //Get header + data 
    uint8_t header_data[net->length+5];
    for(uint8_t i =0; i<sizeof(header_data);i++){
        header_data[i]=*(net_packet+i);
        printf("Header Data %x ",header_data[i]);
        printByte(header_data[i]);
        put_ch(' ');
        
    }
    put_str("\n\r");
    // Print data
    uint8_t data[net->length];
    for(uint8_t i =0; i<sizeof(data);i++){
        data[i]=*(net_packet+5+i);
        printf("App Data %x ",data[i]);
        put_ch((char)data[i]);       
    }
     put_str("\n\r");
    //only print character data
    //put_strHex(header_data,sizeof(header_data));
    // struct' checksum = calculated checksum 
    net->checksum=checksum(header_data,sizeof(header_data),net->ctrl[0] & 0x60);
    printf("Calculated Checksum %x",net->checksum);
    //given_checksum
    uint16_t given_checksum =((uint16_t)*(net_packet+5+net->length)<<8)|*(net_packet+5+net->length+1);
    //compare struct' checksum and given then do what?
    printf("Received %x Checksum",given_checksum); 
    
    //if(net->checksum==given_checksum)
    //    put_str("Correct Checksum");   
}



  
