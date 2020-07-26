#include <EEPROM.h>
#include <Adafruit_Fingerprint.h>

#if defined(__AVR__) || defined(ESP8266)
SoftwareSerial mySerial(2,3);
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

struct Data{
  String option = "0";
  
  int addr = 0;
  byte baca[10]; // length + 1

  String eepromVal;
//  String password;
  String serialVal;

  bool state = false;

  void writeEEPROM(int addr, const String &kata);
  String readPasswordFromEEPROM(int addr);
  void readIsi();
  String initializePassword();
  void enroll();
  void readOption();
  void readFinger();
  void changeFinger();
  void changePassword();

  void readSerial();
}data;

void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  if(finger.verifyPassword()){
    Serial.println("Fingerprint sensor found");
  }else{
    Serial.println("Fingerprint sensor not found");
    while(1){delay(1);}
  }
  finger.getTemplateCount();
  if(finger.templateCount == 0){
    Serial.println("First setup. Enrolling fingerprint");
    data.enroll();
  }else{
    Serial.println("Setup ready");
  }
}

void loop() {
  data.readIsi();
  data.readOption();
  if(data.state){
    if(data.option != "0"){
      data.changePassword();
      data.changeFinger();
    }else{
      data.readFinger();
    }
  }else{
    data.initializePassword();
  }
  delay(10);
}

void Data::readIsi(){
  if(EEPROM.read(0) == 0){
    data.state = false;
  }else{
    data.state = true;
  }
}

void Data::writeEEPROM(int addr, const String &kata){
  byte len = kata.length() + 1;
  EEPROM.write(addr, len);
  for(int i = 0; i < len; i++){
    EEPROM.write(addr + 1 + i, kata[i]);
  }
}

String Data::readPasswordFromEEPROM(int addr){
  data.eepromVal = "";
//  Serial.print("Baca data: ");
  for(int i = addr + 1; i < EEPROM.read(addr); i++){
    data.baca[i-1] = EEPROM.read(i);
  }
  data.eepromVal += String((char *)baca);
//  Serial.println(data.eepromVal);
  return data.eepromVal;
}

String Data::initializePassword(){
  Serial.println("Waiting for password input");
  data.readSerial();
//  data.password = "";
//  while(!Serial.available()){
//    delay(10);
//  }
//  while(Serial.available()){
//    data.password = Serial.readStringUntil('\n');
//  }
  data.writeEEPROM(0,data.serialVal.c_str());
  Serial.println("Device Ready!");
  return data.serialVal;
}

void Data::readOption(){
  while(Serial.available()){
    data.option = Serial.readStringUntil('\n');
    Serial.println(data.option);
  }
}

void Data::enroll(){
  Serial.println("Enrolling fingerprint");
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  Serial.println("remove finger");
  delay(2000);
  p = 0;
  while(p != FINGERPRINT_NOFINGER){
    p = finger.getImage();
  }
  p = -1;
  Serial.println("place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  p = finger.storeModel(1);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

void Data::readFinger(){
  uint8_t p = finger.getImage();
  switch(p){
    case FINGERPRINT_OK:
      break;
    default:
      return p;
  }
  p = finger.image2Tz();
  switch(p){
    case FINGERPRINT_OK:
      break;
    default:
      return p;
  }
  p = finger.fingerSearch();
  if(p == FINGERPRINT_OK){
    Serial.print("Baca data: ");
    data.readPasswordFromEEPROM(0);
    Serial.println(data.eepromVal);
  }else{
    return p;
  }
}

void Data::changePassword(){
  if(data.option == "1"){
    Serial.println("Change Password");
    Serial.print("Old password: ");
    data.readSerial();
    if(data.serialVal == data.readPasswordFromEEPROM(0)){
      Serial.println(data.serialVal);
      Serial.println("New Password: ");
      data.readSerial();
      data.writeEEPROM(0,data.serialVal.c_str());
      data.option = "0";
    }else{
      Serial.println("Password Salah!");
    }
  }
}

void Data::changeFinger(){
  if(data.option == "2"){
    Serial.println("Change Finger");
  }
}

void Data::readSerial(){
  data.serialVal = "";
  while(!Serial.available()){
      delay(10);
    }
    while(Serial.available()){
      data.serialVal = Serial.readStringUntil('\n');
    }
}
