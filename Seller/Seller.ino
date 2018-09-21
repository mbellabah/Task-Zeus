#include <Wire.h>
#include <I2C_Anything.h>

const byte seller_address = 3; 
float q_s; 
int volt_pin = A0; 

void setup(){
  Wire.begin(seller_address);
  pinMode(13, OUTPUT);
  Wire.onRequest(requestSupply);
  Serial.begin(9600);
}

void loop(){
  q_s = calcVoltage();
  delay(1000);
}

void requestSupply(){
  Serial.println("Sending: ");
  Serial.println(q_s);
  I2C_writeAnything(q_s); 
}

float calcVoltage(){
  int sensorValue = analogRead(volt_pin);
  float voltage = sensorValue * (5.0/1023.0);
  return voltage 
}

