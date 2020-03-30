#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* pass = "";
const char* server = "";
const char* username = "";
const char* mqtt_pass = "";
const char* subTopic = "";
const char* pubTopic = "";
uint16_t port;

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
  WiFiConnect();
  client.setServer(server,port);
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
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
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
  
  pubData = "Data Received: ";
  pubData = pubData + (String)counter;
  client.publish(pubTopic,pubData.c_str());
  counter++;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(),username,mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}