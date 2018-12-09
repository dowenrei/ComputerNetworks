
#include "trans.h"
#define ACK_HEADER 7
uint8_t seq=0x00;
uint8_t buffer[APP_MAX]; // for retransmission 

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


void transmit_data(uint8_t* data,uint8_t length,uint8_t* transport_packet,uint8_t isframe){
    struct trans msg;
    uint8_t *t_ptr=transport_packet;
    uint8_t n;

    //First control bit 
    msg.ctrl[0]=0x00; //[0Message,00Checksum,0Unreliable,0NoFragment,0Nth,00]
    #if RELIABLE
    put_str("Using Reliable Transmission -Expecting an Acknowledgement \n\r");
    //set reliable bit
    msg.ctrl[0] |= (1<<4);
    memcpy(buffer,data,length);
    #endif
    //Second control bit
    if(isframe !=0x00)
        msg.ctrl[0] |= 1<<3;    
    msg.ctrl[1]=seq<<2 | (isframe); //[Sequence 6 bits, Fragment 2 bits]

    msg.src=SRC_PORT;
    msg.dest=DEST_PORT;
    msg.length=length;
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


    //concatenate header with data 
    memcpy(to_checksum+5,data,msg.length);

    for(uint8_t i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
    }

    msg.checksum=checksum(to_checksum,sizeof(to_checksum),0x00);   
    printf("\nChecksum %x\n",msg.checksum);
    // MSB, LSB [masking to split msg.checksum to two 8 bits]
    uint8_t checksums[2]={ (uint8_t)(msg.checksum >> 8),(uint8_t)(msg.checksum & 0xFF)};
    
    //to_checksum is now transport_packet after adding the checksum bits
    memcpy(to_checksum+msg.length+5,checksums,sizeof(checksums));
    
    put_str("Transport Segment in Hex \n\r");
    for(uint8_t i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
        *(t_ptr+i)=to_checksum[i];
        printByte(to_checksum[i]);
        put_ch(' ');
    }
    put_str("\n\r");
    //Increase seq number if it's not framed
    if(length!=0x71)
        seq+=1;
    if (seq==64)
        seq=0;
    #if DEBUG
    put_str("Src Port:");
    printByte(msg.src);
    put_str("\n\rDest Port:");
    printByte(msg.dest);
    put_str("\n\rSequence Number:");
    printByte(msg.ctrl[1]>>2);
    put_str("\n\rFragment Number:");
    printByte(msg.ctrl[1] & 0x03);   
    put_str("\n\rMessage Length (including newline):");
    printByte(msg.length);        
    put_str("\n\rData: ");
    put_strHex(data,msg.length);
    put_str("\n\rChecksum: ");
    printByte(to_checksum[msg.length+5]);
    put_ch(' ');
    printByte(to_checksum[msg.length+6]);
    put_str("\n\r");
    #endif

}


void receive(uint8_t* net_packet,uint8_t length){
    //currently didn't use length yet 
    trans* net=(trans*) net_packet;
    //If the communication is reliable, do checksum on original header data, print data to UART, set ack bit to 1 , do checksum for SendAcknowlendgement Bit
    if ( (net->ctrl[0] & (1<<4)) == 0x10){
        put_str("\n\rReceive Block : Reliable \n\r");      
        uint8_t header_data[net->length+5];
        for(uint8_t i =0; i<sizeof(header_data);i++){
        header_data[i]=*(net_packet+i);
        }
        net->checksum=checksum(header_data,sizeof(header_data),net->ctrl[0] & 0x60);
        uint16_t given_checksum_r =((uint16_t)*(net_packet+5+net->length)<<8)|*(net_packet+5+net->length+1);
        if(net->checksum!=given_checksum_r){
        net->ctrl[0] |=1<<0;
        put_str("Checksum Error\n\r");
        }
        net->ctrl[0] |= 1<<7;
        uint8_t trans_header[ACK_HEADER];
        trans_header[0]=net->ctrl[0];
        trans_header[1]=net->ctrl[1];
        trans_header[2]=net->src;
        trans_header[3]=net->dest;
        trans_header[4]=net->length;
        uint16_t ack_checksum=checksum(trans_header,ACK_HEADER-2,net->ctrl[0] & 0x60);
        trans_header[5]= (uint8_t)(ack_checksum >> 8);
        trans_header[6]=(uint8_t)(ack_checksum & 0xFF);
        sendAcknowledgement(trans_header);
    }

    /*
    //Get header + data 
    uint8_t header_data[net->length+5];
    //Print Data only when there is data 
    if (net->length !=0x00){
    for(uint8_t i =0; i<sizeof(header_data);i++){
        header_data[i]=*(net_packet+i);
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
    }
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

  
    #if DEBUG
    put_str("Src Port:");
    printByte(msg.src);
    put_str("\n\rDest Port:");
    printByte(msg.dest);
    put_str("\n\rSequence Number:");
    printByte(msg.ctrl[1]>>2);
    put_str("\n\rFragment Number:");
    printByte(msg.ctrl[1] & 0x03);   
    put_str("\n\rMessage Length (including newline):");
    printByte(msg.length);        
    put_str("\n\rData: ");
    put_strHex(data,msg.length);
    put_str("\n\rChecksum: ");
    printByte(to_checksum[msg.length+5]);
    put_ch(' ');
    printByte(to_checksum[msg.length+6]);
    put_str("\n\r");
    #endif
    */

}

void sendAcknowledgement(uint8_t * trans_header){
    for(uint8_t i =0; i<7;i++){
        printByte(trans_header[i]);
        put_ch(' ');
    }
    put_str("\n\r Acknowledgement Sent\n\r");
}

  
