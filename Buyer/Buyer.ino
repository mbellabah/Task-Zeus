#include <Wire.h>
#include <I2C_Anything.h>

const byte buyer_address = 1;
float q_d; 
int volt_pin = A0; 

void setup() {
  Wire.begin(buyer_address);
  pinMode(13, OUTPUT);
  Wire.onRequest(requestDemand); // Request Handler
  Serial.begin(9600);
}

void loop() {
  q_d = calcVoltage();
  delay(1000); 
}

// Called by interrupt service routine when incoming data arrives

void requestDemand(){
  Serial.print("Sending : ");
  Serial.println(q_d);
  I2C_writeAnything(q_d); 
}

float calcVoltage(){
  int sensorValue = analogRead(volt_pin); 
  float voltage = sensorValue * (5.0/1023.0);
  return voltage; 
}


