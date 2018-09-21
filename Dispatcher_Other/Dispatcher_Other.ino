//MARK: Arduino Libraries
#include <Wire.h>
#include <I2C_Anything.h>


//MARK: C++ Libraries 
#include <StandardCplusplus.h>
#include <string> 
#include <serstream>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <math.h>
#include <list>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <iterator> 

using namespace std;
namespace std{
  ohserialstream cout(Serial);
}

int marketSize = 10; 


//MARK: Classes
class Buyer{
public:
  int address;
  double demand;
  Buyer(int x, double y);
};
Buyer::Buyer(int x, double y){
  this->address = x;
  this->demand = y; 
}

class Seller{
public:
  int address;
  double supply;
  Seller(int x, double y);  
};
Seller::Seller(int x, double y){
  this->address = x;
  this->supply = y;
}


//MARK: Dictionaries (Global) and addresses
std::map<int, Buyer*> buyer_dict;
std::map<int, Seller*> seller_dict;
std::map<string, double> matches_dict;

int buyer_addrs[] = {5, 2};
int buyerSize = sizeof(buyer_addrs)/sizeof(int);

int seller_addrs[] = {3, 4};
int sellerSize = sizeof(seller_addrs)/sizeof(int);


//MARK: Arduino Query Functions
int queryEntity(int address){
  int receivedVal;
  Wire.beginTransmission(address);
  int available_bytes = Wire.requestFrom(address, (uint8_t)2);

  if (available_bytes == 2){
    receivedVal = Wire.read() << 8 | Wire.read();
  }else{
    cout << "Error, unexpected number of bytes received: " << available_bytes << endl;
  }
  Wire.endTransmission();
  return receivedVal; 
}

void execute(int buyer_pin, int seller_pin){
  digitalWrite(seller_pin, HIGH);
  digitalWrite(buyer_pin, HIGH);
  delay(100);
  digitalWrite(seller_pin, LOW);
  digitalWrite(buyer_pin, LOW);
}

//MARK: Matching Related Functions 
void populateEntities(){
  // Deleting all objects
  /*
  for (std::map<int, Buyer*>::iterator buyer_i = buyer_dict.begin(); buyer_i != buyer_dict.end(); ++buyer_i){
    for (std::map<int, Seller*>::iterator seller_i = seller_dict.begin(); seller_i != seller_dict.end(); ++seller_i){
      delete buyer_i->second;
      delete seller_i->second; 
    }
  }
  */
  buyer_dict.clear();
  seller_dict.clear();
  
  for (int i=0; i<buyerSize; i++){
    int buyer_address = buyer_addrs[i];
    int query = (rand() % 100) + 1;
    buyer_dict[buyer_address] = new Buyer(buyer_address, query); 
  }
  for (int i=0; i<sellerSize; i++){
    int seller_address = seller_addrs[i];
    int query = queryEntity(seller_address);
    if (query > 0){
      seller_dict[seller_address] = new Seller(seller_address, query);
    }
  }
}

void makeMatches(){
  matches_dict.clear();
  for (std::map<int, Buyer*>::iterator buyer_i = buyer_dict.begin(); buyer_i != buyer_dict.end(); ++buyer_i){
    for (std::map<int, Seller*>::iterator seller_i = seller_dict.begin(); seller_i != seller_dict.end(); ++seller_i){
      int b_name = buyer_i->first;
      int s_name = seller_i->first;

      string match_name;
      {
        stringstream s1; 
        stringstream s2;
        s1 << b_name;
        s2 << s_name;
        match_name = s1.str() + "," + s2.str();
      }
      double quantity_dev = fabs(buyer_i->second->demand - seller_i->second->supply);
      matches_dict[match_name] = quantity_dev; 
    }
  }
}

int findBest(int buyer_addrs){
  list<double> quantityList;
  list<double>::iterator quantityListIterator;

  double smallest_q;

  if (matches_dict.size() > 0){
    for (std::map<string, double>::iterator matches_i = matches_dict.begin(); matches_i != matches_dict.end(); ++matches_i){
      string full_string = matches_i->first;
      size_t comma_indx = full_string.find(",");
      string buyer_tag = full_string.substr(0, comma_indx);
      int converted = strtol(buyer_tag.c_str(), NULL, 10);

      if (buyer_addrs == converted){
        quantityList.push_back(matches_i->second);
      }
    }
  }

  double temp = 1000.0;
  for (list<double>::iterator qIterator = quantityList.begin(); qIterator != quantityList.end(); qIterator++){
    if (*qIterator < temp){
      temp = *qIterator;
    }
  }
  smallest_q = temp;

  int match_seller;
  for (std::map<string, double>::iterator matches_i = matches_dict.begin(); matches_i != matches_dict.end(); ++matches_i){
    string full_string = matches_i->first;
    size_t comma_indx = full_string.find(",");

    int buyer_tag = strtol(full_string.substr(0,comma_indx).c_str(), NULL, 10);
        int seller_tag = strtol(full_string.substr(comma_indx+1).c_str(), NULL, 10);

        if ((buyer_addrs == buyer_tag) && (matches_i->second == smallest_q)){
                match_seller = seller_tag;
                break;
        }
  }
  return match_seller; 
}


// MARK: Troubleshooting Methods
void findAll(){
   for (std::map<int, Buyer*>::iterator buyer_i = buyer_dict.begin(); buyer_i != buyer_dict.end(); ++buyer_i++){
        int bestOption = findBest(buyer_i->first);
        cout << "B: " << buyer_i->first << " => " << "S: " << bestOption << endl;
        execute(buyer_i->first, bestOption);
        
    }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  srand(time(NULL));

  //MARK: Pin initialization 
  for (int i=0; i<buyerSize; i++){
    pinMode(buyer_addrs[i], OUTPUT);
  }
  for (int i=0; i<sellerSize; i++){
    pinMode(seller_addrs[i], OUTPUT);
  }
}
void (*resetFunc)(void) = 0;

void loop() {
  populateEntities();
  makeMatches();
  findAll();
  delay(300);
  resetFunc();
}
