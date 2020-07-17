#include <ESP8266WiFi.h>
void sendCommand(const char *command);
void connectWiFi();
const char* ssid = "MEMS";
const char* password = "12345678";
const char * host = "192.168.4.1";

const int httpPort = 80;
short unsigned int counter = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("ini setup");
  connectWiFi();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Client-------------------------------");
    sendCommand("RoomID,BedID");
    delay(1000);
    Serial.println("Enter deep sleep");
    ESP.deepSleep(0);
  }else{
    connectWiFi();
  }
}
void sendCommand(const char *command){
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  Serial.print("Sent Message: ");
  Serial.println(command);

  client.print((String)command + "\r");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {      
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  Serial.print("Server Reply = "); 
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("\n-------------------------------------");
  Serial.println("");
}

void connectWiFi(){
  Serial.print("Connecting to Network");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    counter++;
    if(counter >= 20){
      Serial.println("Failed to connect, enter deepsleep");
      ESP.deepSleep(0);
    }
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------------------");
}
