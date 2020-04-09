#include <ESP8266WiFi.h>
#include <PubSubClient.h>

struct{
  const char* ssid = "";
  const char* pass = "";
}Wifi;
struct{
  const char* server = "";
  const char* username = "";
  const char* pass = "";
  const char* subTopic = "";
  const char* pubTopic = "";
  uint16_t port = 13001; // use your own port
}MQTT;

String data;
String pubData;

int counter = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void WiFiConnect();
void getMQTT(char* topic, byte* payload, unsigned int length);
void reconnect();

void setup() {
  Serial.begin(115200);
  //WiFi.mode(WIFI_STA);
  WiFiConnect();
  client.setServer(MQTT.server,MQTT.port);
  client.setCallback(getMQTT);
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    if(!client.connected()){
      reconnect();
    }
    client.loop();
  }else{
    WiFiConnect();
  }
}

void WiFiConnect(){
  Serial.print("Connecting to: ");
  Serial.println(Wifi.ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(Wifi.ssid,Wifi.pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void getMQTT(char* topic, byte* payload, unsigned int length){
  data = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.print("\nData: ");
  Serial.println(data);
  //Publish message after receiving data
  counter++;
  pubData = "Data Received: ";
  pubData = pubData + (String)counter;
  client.publish(MQTT.pubTopic,pubData.c_str());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(),MQTT.username,MQTT.pass)) {
      Serial.println("connected");
      client.subscribe(MQTT.subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
