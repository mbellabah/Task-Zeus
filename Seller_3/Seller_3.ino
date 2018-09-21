#include <Wire.h>
#include <I2C_Anything.h>

const byte seller_address = 3; 
int q_s; 

void setup(){
  Wire.begin(seller_address);
  pinMode(13, OUTPUT);
  Wire.onRequest(requestSupply);
  Serial.begin(9600);
}

void loop(){
}

void requestSupply(){
  int q_s = calcVoltage();
  uint8_t buffer[2];
  buffer[0] = q_s >> 8; 
  buffer[1] = q_s & 0xff;
  Wire.write(buffer, 2); 
}

int calcVoltage(){
  return analogRead(A0);
}

