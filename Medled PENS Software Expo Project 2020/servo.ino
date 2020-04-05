#include <ESP8266WiFi.h>
#include <Servo.h>
#include <CD74HC4067.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#define pin 2
#define led 3
const char* ssid = "ZaCK";
const char* pass = "24446666688888880000";

CD74HC4067 my_mux(16,5,4,0);
CD74HC4067 mux_led(14,12,13,15);
Servo servo;

void connectWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

String parsing(String data){
  String hasil[10];
  int i = 0;
  int indexData = 0;
  hasil[indexData] = "";
  for(i = 0; i < data.length(); i++){
    if(data[i] == ','){
      indexData++;
      hasil[indexData] = "";
    }else{
      hasil[indexData] = hasil[indexData] + data[i];
    }
  }
  return hasil[0];
}

void getData(){
  HTTPClient http;
  http.begin("http://192.168.43.149/MedLed/login/SendToDB.php?id=2");
  int httpCode = http.GET();
  if(httpCode > 0){
    String statuss = http.getString();
    Serial.print("id rak: ");
    //String nilai = getValue(statuss,',',1);
    String nilai = parsing(statuss);
    Serial.println(nilai);
    servo_gerak(nilai);
  }
}

void resetDB(){
  HTTPClient http;
  http.begin("http://192.168.43.149/MedLed/login/kirim.php");
  int httpCode = http.GET();
  Serial.print("http code: ");
  Serial.println(httpCode);
}

void servo_gerak(String rak){
  //my_mux.channel(0);
  if(rak == "0"){
    mux_led.channel(0);
    digitalWrite(led,LOW);
    Serial.println("tidak membuka rak");
    servo.write(1);
    delay(1000);
  }else if( rak == "33"){
    my_mux.channel(1);
    mux_led.channel(1);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 1");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }else if( rak == "34"){
    my_mux.channel(2);
    mux_led.channel(2);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 2");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }else if( rak == "35"){
    my_mux.channel(3);
    mux_led.channel(3);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 3");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }else if( rak == "36"){
    my_mux.channel(4);
    mux_led.channel(4);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 4");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }else if( rak == "38"){
    my_mux.channel(5);
    mux_led.channel(5);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 5");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }else if( rak == "39"){
    my_mux.channel(6);
    mux_led.channel(6);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 6");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }
  else if( rak == "40"){
    my_mux.channel(7);
    mux_led.channel(7);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 7");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }
  else if( rak == "41"){
    my_mux.channel(8);
    mux_led.channel(8);
    digitalWrite(led,HIGH);
    Serial.println("membuka rak 8");
    real_servo_gerak();
    delay(1000);
    resetDB();
  }
  Serial.println("");
}

void real_servo_gerak(){
  servo.write(90);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  pinMode(pin,OUTPUT);
  servo.attach(pin);
  connectWiFi();
}

void loop() {
  digitalWrite(pin,LOW);
  if(WiFi.status() == WL_CONNECTED){
    getData();
  }else{
    connectWiFi();
  }
}
