#include <CloudX/M633.h>
#include <CloudX/Serial.h>
#include "SPI.h"
#define slave1 1


unsigned long spi_bytes;
byte byte_0=0,byte_1=0;
char text[7];
setup(){
    pinMode(slave, OUTPUT);
    SPI_begin(master);
    Serial_begin(9600);
    loop(){
        SPI_beginTransaction(SPI_MODE2,1000);
        digitalWrite(slave, LOW);
   // SPI_write(0x09); // read firt 8 bits
  // read second 8 bits
    byte_0 = SPI_transfer(0x09);
    byte_1= SPI_transfer(0);
    digitalWrite(slave, HIGH);
     spi_bytes = ( ( (byte_0 & 0b00011111) << 3) | (byte_1 >>1) ); 
     intTostr(text,spi_bytes, DEC);
       Serial_writeText(text);
         Serial_writeText("\r \n");
    digitalWrite(slave1, HIGH);
    }

}