//#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>

uint8_t app[114];

struct trans{
    uint8_t ctrl[2];
    uint8_t src;
    uint8_t dest;
    uint8_t length;
    uint16_t checksum; //need?
    //data?
};

//not the correct function but still trying
uint8_t transmit(uint8_t* data){
    trans* msg=(trans*) data;
    msg->checksum=data[1];
    //printf("%u",msg->checksum);
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

int main(){
    uint8_t trans_header[4]={0,1,2,3};    
    uint8_t ans;
    ans=transmit(trans_header);
    //aa46 not aa12
    uint8_t parity_test[8] = {0x01, 0x10, 0xab, 0xbc, 0xef, 0xbe, 0xef,0x54};
    checksum(parity_test,sizeof(parity_test));
} 


// get two last bytes of data

  
