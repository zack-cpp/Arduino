#include <Keyboard.h>

#include <Keypad.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

char KeysID[] = {'1',  '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
char keys[ROWS][COLS] = {
  {KeysID[0],  KeysID[1], KeysID[2], KeysID[3]},
  {KeysID[4],  KeysID[5], KeysID[6], KeysID[7]},
  {KeysID[8],  KeysID[9], KeysID[10], KeysID[11]},
  {KeysID[12],  KeysID[13], KeysID[14], KeysID[15]}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

String kata;
String kata2;
void setup() {
  Keyboard.begin();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Data:");
}

void loop() {
  lcd.setCursor(0,1);
  char key = keypad.getKey();
  if(key){
    if(key != KeysID[15]){
      kata += key;
      for(int i = 0; i < kata.length(); i++){
        lcd.setCursor(i,1);
        lcd.print(kata[i]);
      }
    }else{
      for(unsigned short i = 0; i < kata.length(); i++){
        Keyboard.write(kata[i]);
        delay(100);
      }
      delay(200);
      Keyboard.write("\n");
      kata = "";
    }
  }
  delay(100);
}
