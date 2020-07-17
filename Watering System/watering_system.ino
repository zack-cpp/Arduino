#include <Keypad.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define pump 10

unsigned int time = 0;;

const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

bool state = false;
bool watering = false;
bool wateringDone = false;
bool buttonPressed = false;

String input = "";
String jam[2],menit[2];
int iJam[2],iMenit[2],counter = 0, wateringCounter = 0;
char KeysID[] = {'1',  '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
char keys[ROWS][COLS] = {
  {KeysID[0],  KeysID[1], KeysID[2], KeysID[3]},
  {KeysID[4],  KeysID[5], KeysID[6], KeysID[7]},
  {KeysID[8],  KeysID[9], KeysID[10], KeysID[11]},
  {KeysID[12],  KeysID[13], KeysID[14], KeysID[15]}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void checkTime();
void getClock();
void setCurrentTime();
void setWateringTime();
void startClock();
void decodeClock();
void startWatering();
void wateringTimer();

void setup() {
  Serial.begin(9600);
  pinMode(pump,OUTPUT);
  digitalWrite(pump,HIGH);
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  setCurrentTime();
  decodeClock();
  setWateringTime();
  decodeClock();
}

void loop() {
  startClock();
  checkTime();
  char key = keypad.getKey();
  if(key){
    if(key == KeysID[12]){
      buttonPressed = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Watering Time!!!");
      startWatering();
    }
  }
  if(time == 0){
      // FOR DEBUGGING
//    Serial.print("Current Time: \t");
//    Serial.print(iJam[0]);
//    Serial.print(":");
//    Serial.print(iMenit[0]);
//    Serial.print(":");
//    Serial.println(counter);
//    Serial.print("Watering Time:\t");
//    Serial.print(iJam[1]);
//    Serial.print(":");
//    Serial.println(iMenit[1]);
    if(watering){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Watering Time!!!");
//      Serial.println("Watering Time!!!");
    }else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Watering at:");
      lcd.setCursor(0,1);
      lcd.print(iJam[1]);
      lcd.print(":");
      lcd.print(iMenit[1]);
    }
  }
}

void setCurrentTime(){
  lcd.clear();
  lcd.print("Current Time");
  input = "";
  getClock();
  state = true;
}

void setWateringTime(){
  lcd.clear();
  lcd.print("Watering Time");
  input = "";
  getClock();
  state = false;
}

void getClock(){
  lcd.setCursor(2,2);
  lcd.print(":");
  unsigned int jml = 0;
  while(true){
    char key = keypad.getKey();
    if(key){
      if(key != KeysID[3] && key != KeysID[7] && key != KeysID[11] && key != KeysID[12] && key != KeysID[14] && key != KeysID[15]){
        if(jml == 2){
          input += ":";
          jml++;
        }
        input += key;
        jml++;
        for(unsigned int x = 0; x < input.length(); x++){
          lcd.setCursor(x,1);
          lcd.print(input[x]);
        }
        if(jml == 5){
//            Serial.println("break");
            break;
        }
      }
    }
  }
}
void decodeClock(){
  if(state){
    jam[0] += input[0];
    jam[0] += input[1];
    iJam[0] = jam[0].toInt();
    menit[0] += input[3];
    menit[0] += input[4];
    iMenit[0] = menit[0].toInt();
    if(iJam[0] > 24 || iMenit[0] > 60){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INPUT SALAH");
      iJam[0] = 0;
      iMenit[0] = 0;
      setCurrentTime();
    }
  }else{
    jam[1] += input[0];
    jam[1] += input[1];
    iJam[1] = jam[1].toInt();
    menit[1] += input[3];
    menit[1] += input[4];
    iMenit[1] = menit[1].toInt();
    if(iJam[1] > 24 || iMenit[1] > 60){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INPUT SALAH");
      setWateringTime();
    }
  }
}
void checkTime(){
  if(iJam[0] == iJam[1] && iMenit[0] == iMenit[1] && wateringCounter < 10 && wateringDone == false || buttonPressed == true){
    startWatering();
    wateringDone = true;
  }
  else if(iJam[0] == iJam[1]){
    if(iMenit[0] != iMenit[1]){
      wateringDone = false;
    }
  }
  wateringTimer();
}
void startClock(){
  delay(10);
  time++;
  if(time == 100){
    counter++;
    if(watering){
      wateringCounter++;
    }
    time = 0;
  }
  if(counter == 60){
    counter = 0;
    iMenit[0]++;
  }
  if(iMenit[0] == 60){
    iMenit[0] = 0;
    iJam[0]++;
  }
  if(iJam[0] == 24){
    iJam[0] = 0;
  }
}

void startWatering(){
  watering = true;
  digitalWrite(pump,LOW);    //ACTIVE LOW
}

void wateringTimer(){
  if(wateringCounter == 10){
    watering = false;
    wateringCounter = 0;
    digitalWrite(pump,HIGH); //ACTIVE LOW
    if(buttonPressed = true){
      buttonPressed = false;
    }
  }
}
