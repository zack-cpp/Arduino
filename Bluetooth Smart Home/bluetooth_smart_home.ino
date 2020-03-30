#define kamar1 4
#define kamar2 5
#define kamar3 6
#define kamar4 7

String dataIn;
String data[4];
boolean state = false;
int statuss[4];
void setup() {
  Serial.begin(9600);
  pinMode(kamar1, OUTPUT);
  pinMode(kamar2, OUTPUT);
  pinMode(kamar3, OUTPUT);
  pinMode(kamar4, OUTPUT);
}
void loop(){
  unsigned int i;
  unsigned int jml_kata = 0;
  if(Serial.available()){
    char kataIn = (char)Serial.read();
    dataIn += kataIn;
    if(kataIn == '#'){
      state = true;
    }
  }
  if(state){
    Serial.print("dataIn: ");
    Serial.println(dataIn);
    int data_now = 0;
    data[data_now] = "";
    for(i = 0; i < dataIn.length(); i++){
      if(dataIn[i] == '#' || dataIn[i] == ','){
        data_now++;
        data[data_now] = "";
      }else{
        data[data_now] += dataIn[i];
      }
    }
    for(i = 0; i < 4; i++){
      Serial.print("\ndata-");
      Serial.print(i);
      Serial.print(": ");
      statuss[i] = data[i].toInt();
      Serial.println(statuss[i]);
      if(statuss[i] == 0){
        if(i == 0){
          digitalWrite(kamar1, LOW);
          Serial.println("lampu kamar 1 mati");
        }else if(i == 1){
          digitalWrite(kamar2, LOW);
          Serial.println("lampu kamar 2 mati");
        }else if(i == 2){
          digitalWrite(kamar3, LOW);
          Serial.println("lampu kamar 3 mati");
        }else if(i == 3){
          digitalWrite(kamar4, LOW);
          Serial.println("lampu kamar 4 mati");
        }
      }else if(statuss[i] == 1){
        if(i == 0){
          digitalWrite(kamar1, HIGH);
          Serial.println("lampu kamar 1 nyala");
        }else if(i == 1){
          digitalWrite(kamar2, HIGH);
          Serial.println("lampu kamar 2 nyala");
        }else if(i == 2){
          digitalWrite(kamar3, HIGH);
          Serial.println("lampu kamar 3 nyala");
        }else if(i == 3){
          digitalWrite(kamar4, HIGH);
          Serial.println("lampu kamar 4 nyala");
        }
      }
    }
    dataIn = "";
    state = false;
  }
}
