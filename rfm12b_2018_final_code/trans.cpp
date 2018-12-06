//gcc trans.cpp -o trans && ./trans
//#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
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

//not the correct function but still trying, assume data is word to be transmitted
uint8_t transmit(uint8_t* data){
    printf("Data %u",data[3]);
    //struct trans msg;
    trans* msg=(trans*) data;
    //after this , data array will be same as msg array 
    printf("Checksum %u",msg[1]);
    uint8_t n;
    //do control
    msg->ctrl[0]=0x07;
    msg->ctrl[1]=0x0b;

    msg->src=SRC_PORT;
    msg->dest=DEST_PORT;
    msg->length=sizeof(data);
    n=sizeof(data)+5;
    //uint8_t to_checksum[n]={msg->ctrl[0],msg->ctrl[1],msg->src,msg->dest,msg->length,data};
    //printf("Checksum value : %u",to_checksum[0]);
    printf("Checksum %u",data[3]);
    

    return 8; 
}

// can add case 
uint16_t checksum(uint8_t* data,uint8_t length){
    uint16_t parity=0x0000;
    for(int i=0;i<length;i++){
        printf("%i",i);
        if (i%2==0){
            parity^=(uint16_t)data[i]<<8;
            printf("msb %x\n",parity);
        }    
        else {         
            parity^=data[i];
            printf("lsb %x\n",parity);           
        }
    }
    return parity;

}
uint8_t receive(uint8_t* data){
    uint8_t *ptr=data;
    *(ptr+1)={0x02};
    return 0;
}



int main(){
    uint8_t trans_header[4]={0,1,2,5};    
    uint8_t ans;
    //ans=transmit(trans_header);
    //aa46 not aa12 
    // Network. Header =0110, 
    uint8_t network_packet[8] = {0x01, 0x10, 0xab, 0xbc, 0xef, 0xbe, 0xef,0x54};
    //ans=receive(network_packet);
    printf("lsb %x\n",network_packet[1]);  
    //uint8_t parity_test[8] = {0x01, 0x10, 0xab, 0xbc, 0xef, 0xbe, 0xef,0x54};
    
    //checksum(parity_test,sizeof(parity_test));
    uint8_t test = 1<<5;
    printf("Test %d ",test);

       /*
    char c='a';
    uint8_t str[114];

    str[0]=c;
    printf("%x",str[0]);

    str[1]=0x62;
    printf("%c",str[1]);
    
    
    for(int i =0; i<sizeof(app_data);i++){
        printf("Trans %x",transport_packet[i]);
    }
    */

    //aa46 not aa12 
    // Network. Header =0110, 
    //uint8_t network_packet[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00};
    //uint8_t parity_test[8] = {0x01, 0x10, 0xab, 0xbc, 0xef, 0xbe, 0xef,0x54};
    
    //checksum(parity_test,sizeof(parity_test));

} 


// get two last bytes of data

  
