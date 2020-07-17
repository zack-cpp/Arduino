#include <ESP8266WiFi.h>

String answer = "";
const char *ssid = "MEMS";
const char *password = "12345678";
const char* Commands_Reply;

//const char * host = "192.168.4.2";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

//  Serial.println("");
//  Serial.println("Server-------------------------------");
//  Serial.print("Configuring access point");
  WiFi.softAP(ssid,password);

  server.begin();
//  Serial.println("Server started");
//  Serial.print("Use this URL to connect: ");
//  Serial.print("http://");  
//  Serial.print(WiFi.softAPIP());
//  Serial.println("/");
//  Serial.println("-------------------------------------");
//  Serial.println("");
}

void loop() {
  WiFiClient client = server.available();
  while(Serial.available()){
    char s = Serial.read();
    answer += s;
  }
  if(answer == "Confirmed"){
    
  }
  answer = "";
  delay(100);
  if (!client) {
    return;
  }
  // Wait until the client sends some data
//  Serial.println("Server-------------------------------");
//  Serial.print("Arrived Message: ");
  while(!client.available()){
    Serial.println("there");
    delay(1);
  }
  String req = client.readStringUntil('\r');
//  Serial.println(req);
  client.flush();

  if (req != ""){
    Commands_Reply = "Oke_gan";
//    Serial.print("Server send = ");
//    Serial.println(Commands_Reply);
    client.print((String)Commands_Reply+"\r");
//    Serial.print("\nData: ");
    Serial.write(req.c_str());
  }
  client.flush();
//  Serial.println("\nClient disonnected");
//  Serial.println("-------------------------------------");
//  Serial.println("");
}
