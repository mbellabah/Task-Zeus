#include <Wire.h>
#include <I2C_Anything.h>

const byte buyer_address = 1;
int q_d; 

void setup() {
  Wire.begin(buyer_address);
  Wire.onRequest(requestDemand); // Request Handler
  Serial.begin(9600);
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

int calcVoltage(){ // 2.5 and below registers as a zero 
  return analogRead(A0); 
}


