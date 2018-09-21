#include <Wire.h>
#include <I2C_Anything.h>

int buyer_size = 2;
int buyer_addrs[] = {
  5,
  2
};

int seller_size = 2; 
int seller_addrs[] = {
  3,
  4
};


int queryEntity(int addr){
  int receivedValue;
  Wire.beginTransmission(addr);
  int available_bytes = Wire.requestFrom(addr, (uint8_t)2);

  if (available_bytes == 2){
    receivedValue = Wire.read() << 8 | Wire.read();
  }else{
    Serial.print("ERROR: Unexpected number of bytes recieved: ");Serial.println(available_bytes);
  }
  
  Wire.endTransmission();
  return receivedValue;
}



void queryMarket(){
  for (int i = 0; i < buyer_size; i++){
    Serial.print("B");Serial.print(buyer_addrs[i]);
    Serial.print(" ");Serial.println(queryEntity(buyer_addrs[i])); 
  }
  for (int i = 0; i < seller_size; i++){
    Serial.print("S");Serial.print(seller_addrs[i]);
    Serial.print(" ");Serial.println(queryEntity(seller_addrs[i]));
  }
}


void setup(){
  Wire.begin();
  pinMode(13, OUTPUT);

  for (int i = 0; i < buyer_size; i++){
    pinMode(buyer_addrs[i], OUTPUT);
  }

  for (int i = 0; i < seller_size; i++){
    pinMode(seller_addrs[i], OUTPUT);
  }
  
  Serial.begin(115200);
}

void loop(){
  queryMarket();
  sendRequest();
}



void sendRequest(){ // Receives which gate to open, 
  for (int i = 0; i < buyer_size; i++){
    Serial.print("!");Serial.println(buyer_addrs[i]); 
    String match_seller = Serial.readString(); 
    if (match_seller.length() > 0){
      int seller_pin = match_seller.toInt();
      execute(buyer_addrs[i], seller_pin);
    }
  }
}

void execute(int buyer_pin, int seller_pin){
  digitalWrite(seller_pin, HIGH);
  digitalWrite(buyer_pin, HIGH);
  delay(2000); 
  digitalWrite(seller_pin, LOW);
  digitalWrite(buyer_pin, LOW);
  delay(2000); 
}



