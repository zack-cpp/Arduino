const unsigned int jmlData = 3; //jumlah data yang diinginkan

String dataString;
String data[jmlData];

void parsing();

void setup() {
  Serial.begin(9600);
}
void loop() {
  while(!Serial.available()){
    delay(500);
  }
  while(Serial.available()){
    char readSerial = (char)Serial.read();
    dataString += readSerial;
  }
  parsing();
}
void parsing(){
  Serial.print("Data: ");
  Serial.println(dataString);
  unsigned int i = 0, dataNow = 0;
  data[dataNow] = "";
  for(i = 0; i < dataString.length(); i++){
    if(dataString[i] == ','){
      dataNow++;
      data[dataNow] = "";
    }else{
      data[dataNow] += dataString[i];
    }
  }
  for(i = 0; i < jmlData; i++){
    Serial.print("Data[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(data[i]);
  }
  dataString = "";
}
