
#define SRC_PORT 21
#define DEST_PORT 21 
#define TRANS_MAX 114



struct trans{
    uint8_t ctrl[2];
    uint8_t src;
    uint8_t dest;
    uint8_t length;
    uint16_t checksum; //need?
    //data?
};

// can add case 
uint16_t checksum(uint8_t* data,uint8_t length){
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
void transmit_data(uint8_t* data,uint8_t length,uint8_t* transport_packet){

    struct trans msg;
    uint8_t *t_ptr=transport_packet;
    //trans* msg=(trans*) data;
    //uint8_t n;
    //do control
    msg.ctrl[0]=0x07;
    msg.ctrl[1]=0x0b;

    msg.src=SRC_PORT;
    msg.dest=DEST_PORT;
    msg.length=length;
    //cannot create variable size length, assume message is 6
    //n=6+7;

    //concatenate array for checksum, to checksum array varies according to the app_data size 
    uint8_t to_checksum[13]={msg.ctrl[0],msg.ctrl[1],msg.src,msg.dest,msg.length};
    memcpy(to_checksum+5,data,msg.length);

    for(uint8_t i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
        printByte(app_data[0]);
    }



    msg.checksum=checksum(to_checksum,sizeof(to_checksum));   
    printf("\nChecksum %x\n",msg.checksum);
    // MSB, LSB [masking to split msg.checksum to two 8 bits]
    uint8_t checksums[2]={msg.checksum >> 8,msg.checksum & 0xFF};
    
    //to_checksum is now transport_packet after adding the checksum bits
    memcpy(to_checksum+msg.length+5,checksums,sizeof(checksums));
 
     for(uint8_t i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
        *(t_ptr+i)=to_checksum[i];

    }
}


void receive(uint8_t* net_packet,uint8_t length){
    //currently didn't use length yet 
    trans* net=(trans*) net_packet;

    //printf("Received %x huh",*(net_packet+1)); works
    printf("Length %x ",net->length);
    //First Data is always net_packet+5
    printf("First Data %x ",*(net_packet+5));
    //calculate data checksum 
    uint8_t app_data[net->length+5];
    for(uint8_t i =0; i<sizeof(app_data);i++){
        app_data[i]=*(net_packet+i);
        //printf("App Data %x ",app_data[i]);
    }
    // struct' checksum = calculated checksum 
    net->checksum=checksum(app_data,sizeof(app_data));
    printf("Calculated Checksum %x",net->checksum);
    //given_checksum
    uint16_t given_checksum =((uint16_t)*(net_packet+5+net->length)<<8)|*(net_packet+5+net->length+1);
    //compare struct' checksum and given then do what?
    printf("Received %x Checksum",given_checksum); 
}



  
