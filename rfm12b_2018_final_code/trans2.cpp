//#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
//memcpy need this?
#include<string.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>

#define SRC_PORT 21
#define DEST_PORT 21 
uint8_t app[114];


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
uint8_t transmit(uint8_t* data,uint8_t length){
    
    //printf("Data %d ",data[4]);
    struct trans msg;
    //trans* msg=(trans*) data;
    uint8_t n;
    //do control
    msg.ctrl[0]=0x07;
    msg.ctrl[1]=0x0b;
    msg.src=SRC_PORT;
    msg.dest=DEST_PORT;
    msg.length=length;
    n=length+7;

    //concatenate array for checksum 
    uint8_t to_checksum[n]={msg.ctrl[0],msg.ctrl[1],msg.src,msg.dest,msg.length};
    memcpy(to_checksum+5,data,msg.length);

    for(int i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
    }
    msg.checksum=checksum(to_checksum,sizeof(to_checksum));   
    printf("\nChecksum %x\n",msg.checksum);
    // MSB, LSB [masking to split msg.checksum to two 8 bits]
    uint8_t checksums[2]={msg.checksum >> 8,msg.checksum & 0xFF};
    
    //to_checksum is transport_packet
    memcpy(to_checksum+msg.length+5,checksums,sizeof(checksums));
     for(int i =0; i<sizeof(to_checksum);i++){
        printf("%x ",to_checksum[i]);
    }
    
    return 8; 
}


uint8_t receive(uint8_t* data){
    
}

int main(){
    uint8_t app_data[6]={0x67,0x01,0x02,0x05,0x01,0x50};    
    uint8_t ans;
    //printf("Data %d",sizeof(app_data));
    ans=transmit(app_data,sizeof(app_data));
    //aa46 not aa12 
    // Network. Header =0110, 
    //uint8_t network_packet[8] = {0x01, 0x10, 0xab, 0xbc, 0xef, 0xbe, 0xef,0x54};
    //uint8_t parity_test[8] = {0x01, 0x10, 0xab, 0xbc, 0xef, 0xbe, 0xef,0x54};
    
    //checksum(parity_test,sizeof(parity_test));
} 


// get two last bytes of data by masking?

  
