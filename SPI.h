#ifndef _SPI_H_
#define _SPI_H_

#include <stdio.h>
#include <stdlib.h>

void SPI_begin(byte i);
void SPI_beginTransaction(byte mode, unsigned long clockspeed );
unsigned char SPI_write(char data);
unsigned char SPI_read();
unsigned char SPI_transfer(char data);
void SPI_end();

#define master 0
#define slave 1 
#define MODE0 2
#define MODE1 3
#define MODE2 4
#define MODE3 5

enum SPIMode{mastermode, slavemode, SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3};

void SPI_begin(byte i){
switch(i){

    case mastermode:                         //master mode
     TRISC5 = 0;                   //SDO AS OUTPUT
     TRISA5 = 0;                  // SS OUTPUTS IN MASTER MODE
     SSPSTAT = 0b00000000;
     SSPCON = 0b00100000; 
     TRISC3 = 0;                   //SCK AS OUTPUT
     ADCON1 =0x04;
     break;
    case slavemode:         //slave mode
     TRISC5 = 0;  
    TRISC4=1;
     SSPSTAT = 0x00; 
     SSPCON = 0x24; 
     TRISC3 = 1;
     ADCON1 =0x04;
     break;
    
}    
}
// the SPI FOR cloudx m633 has a maximum clock of 20,000,000/4 = 5Mbps;
void SPI_beginTransaction(byte mode, unsigned long clockspeed ) {    
    switch (mode)
    {
         case SPI_MODE0:    //output falling Datacapture Rising
             CKP = 0;
             CKE = 0;
             break;
        case SPI_MODE1:
            CKP= 0;
            CKE = 1;
            break;
        case SPI_MODE2:
            CKP = 1;
            CKE = 0;
            break;
        case SPI_MODE3:
            CKP = 1;
            CKE = 1;
            break;
    }
    if(mode == master){
    if ( clockspeed >= 5000000) {
    SSPM3=0; SSPM2=0; SSPM1=0; SSPM0=0;         
    }
    else if( clockspeed < 5000000 && clockspeed >= 1250000 ){
    SSPM3=0; SSPM2=0;SSPM1=0;SSPM0=1;
       }
    else if (clockspeed < 1250000){
     SSPM3=0; SSPM2=0; SSPM1=1; SSPM0=0;
   }
    }
    }

unsigned char SPI_write(char data) 
{
    SSPCONbits.WCOL = 0;
    PIR1bits.SSPIF = 0;
    SSPBUF = data; //Write the user given data into buffer
    if(SSPCON & 0x80) return (0); //if WCOL bit is not set return 0
    else
        while(!PIR1bits.SSPIF); //wait until bus cycle completes
            return(1);    
}

unsigned char SPI_transfer(char data) {
    SPI_write(data);
    return SPI_read();
}
unsigned char SPI_read() //Read the received data
{
    unsigned char temporaryVar;
    temporaryVar = SSPBUF;   //CLEAR BF
    PIR1bits.SSPIF = 0;      //CLEAR INTERRUPT FLAG
    SSPBUF = 0x00;          //INITIATE BUS CYCLE 
    while(!PIR1bits.SSPIF); // WAIT UNTIL CYCLE COMPLETES
    return(SSPBUF); // RETURN BYTE READ
    while ( !SSPSTATbits.BF );// Hold till BF bit is set, to make sure the complete data is read
    
    
    return(SSPBUF); // return the read data
}
 // Disable the SPI bus
void SPI_end() {
    SSPEN = 0;
}
unsigned char Ready2read(){
    if (SSPSTATbits.BF) return (1);
    else return(0);
}

#endif  //#ifndef _SPI_H_

/*
 *          ckp        cke      output-edge  data-capture
 mode0 -->  0           0       falling-edge    Rising
 mode1 -->  0           1       rising          falling
 mode2 -->  1           0       rising          falling
 mode3 -->  1           1       falling         Rising
 * 
 * */
