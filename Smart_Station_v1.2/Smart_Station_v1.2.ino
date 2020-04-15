//  Name    : Smart Station v1.2
//  Creator : ZaCK
//  Email   : neojoyko@gmail.com
//  GitHub  : https://github.com/zack-cpp/Arduino
//  Desc    : Smart station with Nextion Display integrated with
//            WeMos D1 mini or other ESP8266 board. Connected
//            with MQTT protocol easily IoT control. Updates
//            will be release soon. If you want to get the
//            "myNextion.h" library or the HMI file, please contact 
//			  me on email.

#include <ESP8266WiFi.h>
#include <myNextion.h>
#include <PubSubClient.h>
#define LAMPU_KAMAR 16
#define LAMPU_TAMU  14
struct Config{
  String wifiConfig[2];
  String NexData[2];
}conf;

struct{
  String data;
  String pubData;
  const char* server = "tailor.cloudmqtt.com";
  //const char *server = "broker.mqtt-dashboard.com";
  const char* username = "ghavsonc";
  const char* pass = "1FBDeHNad_kb";
  const char* subTopic1 = "subKamar";
  const char* subTopic2 = "subTamu";
  const char* pubTopic1 = "statusKamar";
  const char* pubTopic2 = "statusRuangTamu";
  uint16_t port = 13001;
  //uint16_t port = 1883;
}MQTT;

NextionText status("status");
WiFiClient espClient;
PubSubClient client(espClient);

void getWiFiConfig();
void parsing();
void connectWiFi();
void getMQTT(char* topic, byte* payload, unsigned int length);
void MQTTreconnect();

void setup(){
  Serial.begin(9600);
  pinMode(LAMPU_KAMAR,OUTPUT);
  pinMode(LAMPU_TAMU,OUTPUT);
  conf.wifiConfig[0] = "";
  conf.wifiConfig[1] = "";
  getWiFiConfig();
  conf.wifiConfig[0] = conf.NexData[0];
  conf.wifiConfig[1] = conf.NexData[1];
  connectWiFi();
  client.setServer(MQTT.server,MQTT.port);
  client.setCallback(getMQTT);
}
void loop(){
  if(WiFi.status() == WL_CONNECTED){
    if(!client.connected()){
      reconnect();
    }
    String data;
    while(!Serial.available()){
      delay(100);
      client.loop();
      data = NexResponse.ResponseAndMQTT();
      if(data != ""){
        parsing(data);
        break;
      }else if(MQTT.data != ""){
        break;
      }
    }
    if(conf.NexData[0] == "kamar" || MQTT.data.length() != 0){
      if(conf.NexData[1] == "1" || MQTT.data == "kamar on"){
        digitalWrite(LAMPU_KAMAR,HIGH);
        Serial.println("Kamar Nyala");
        client.publish(MQTT.pubTopic1,"Kamar Nyala");
      }else if(conf.NexData[1] == "0" || MQTT.data == "kamar off"){
        digitalWrite(LAMPU_KAMAR,LOW);
        Serial.println("Kamar Mati");
        client.publish(MQTT.pubTopic1,"Kamar Mati");
      }
    }
    if(conf.NexData[0] == "tamu" || MQTT.data.length() != 0){
      if(conf.NexData[1] == "1" || MQTT.data == "tamu on"){
        digitalWrite(LAMPU_TAMU,HIGH);
        Serial.println("Ruang Tamu Nyala");
        client.publish(MQTT.pubTopic2,"Ruang Tamu Nyala");
      }else if(conf.NexData[1] == "0" || MQTT.data == "tamu off"){
        digitalWrite(LAMPU_TAMU,LOW);
        Serial.println("Ruang Tamu Mati");
        client.publish(MQTT.pubTopic2,"Ruang Tamu Mati");
      }
    }
    MQTT.data = "";
    conf.NexData[0] = "";
    conf.NexData[1] = "";
  }else{ // if not connected to wifi
    NexCommand.sendCommand("page 2");
    NexCommand.sendCommand("page2.pic=7");
    NexCommand.sendCommand("vis b1,1");
    NexCommand.sendCommand("vis b2,1");
    NexCommand.sendCommand("vis t0,1");
    NexCommand.sendCommand("tsw ssid,0");
    NexCommand.sendCommand("tsw pass,0");
    NexCommand.sendCommand("tsw b0,0");
    status.setText("No WiFi");
    if(NexResponse.getResponse() == "yes"){
      NexCommand.sendCommand("page2.pic=3");
      NexCommand.sendCommand("vis b1,0");
      NexCommand.sendCommand("vis b2,0");
      NexCommand.sendCommand("vis t0,0");
      status.setText("Connecting.");
      connectWiFi();
    }else{
      NexCommand.sendCommand("page2.pic=3");
      status.setText("Connect to a WiFi");
      NexCommand.sendCommand("vis b1,0");
      NexCommand.sendCommand("vis b2,0");
      NexCommand.sendCommand("vis t0,0");
      NexCommand.sendCommand("tsw ssid,1");
      NexCommand.sendCommand("tsw pass,1");
      NexCommand.sendCommand("tsw b0,1");
      getWiFiConfig();
      connectWiFi();
    }
  }
  delay(1000);
}
void connectWiFi(){
  WiFi.begin(conf.wifiConfig[0].c_str(),conf.wifiConfig[1].c_str());
  Serial.print("Connecting to: ");
  Serial.println(conf.wifiConfig[0]);
  Serial.print("Pass: ");
  Serial.println(conf.wifiConfig[1]);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");
  Serial.print("IP Addess: ");
  Serial.println(WiFi.localIP());
  status.setText("conected");
  delay(5000);
  NexCommand.sendCommand("page 1");
}
void getWiFiConfig(){
  String dataReceived = NexResponse.getResponse();
  while(dataReceived.length() < 5){
    dataReceived = NexResponse.getResponse();
    parsing(dataReceived);
  }
}
void parsing(String dataReceived){
  unsigned int i = 0, dataNow = 0;
  conf.NexData[dataNow] = "";
  for(i = 0; i < dataReceived.length(); i++){
    if(dataReceived[i] == ','){
      dataNow++;
      conf.NexData[dataNow] = "";
    }else{
      conf.NexData[dataNow] += dataReceived[i];
    }
  }
}
void getMQTT(char* topic, byte* payload, unsigned int length){
  MQTT.data = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    MQTT.data += (char)payload[i];
  }
  Serial.print("\nData: ");
  Serial.println(MQTT.data);
}
void reconnect(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(),MQTT.username,MQTT.pass)) {
      Serial.println("connected");
      client.subscribe(MQTT.subTopic1);
      client.subscribe(MQTT.subTopic2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    if(WiFi.status() != WL_CONNECTED){
      break;
    }
  }
}
