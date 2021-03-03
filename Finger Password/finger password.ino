#include <Keyboard.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

const String pass = "123456";

char toSend[sizeof(pass)];

bool readState = false;

bool readFinger();

SoftwareSerial mySerial(8,9);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  if(finger.verifyPassword()){
    Serial.println("Fingerprint found");
  }else{
    Serial.println("Fingerprint not found");
    while(1){
      delay(1);
    }
  }
}

void loop() {
  if(readFinger()){
    pass.toCharArray(toSend, sizeof(toSend));
    for(byte i = 0; i < sizeof(toSend); i++){
      Keyboard.write(toSend[i]);
      delay(50);
    }
    Keyboard.write(KEY_RETURN);
  }
}

bool readFinger(){
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
    return true;
    // readState = true;
  }else{
    return false;
  }
}