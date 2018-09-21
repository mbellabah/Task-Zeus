#include <Wire.h>
#include <I2C_Anything.h>

const byte buyer_address = 2;
int q_d; 


void setup() {
  Wire.begin(buyer_address);
  Wire.onRequest(requestDemand); // Request Handler
}

void loop() {
}

// Called by interrupt service routine when incoming data arrives

void requestDemand(){
  int q_d = calcVoltage();
  uint8_t buffer[2];
  buffer[0] = q_d >> 8;
  buffer[1] = q_d & 0xff;
  Wire.write(buffer, 2); 
}

int calcVoltage(){
  return analogRead(A0); 
}


