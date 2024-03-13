#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "RBDdimmer.h"

#define zerocross D1
#define outputPin D2

const char* ssid = "WIFI_SSID";
const char* password =  "WIFI_PASSWORD";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
dimmerLamp dimmer(outputPin, zerocross);

void callback(char* topic, byte* payload, unsigned int length) {
    String command = "";
      
    for (int i = 0; i < length; i++) {
      command += (char)payload[i];
   }

   static String previousCommand = "";

  if (command != previousCommand) {
    
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    
    if (strcmp(command.c_str(), "on") == 0) {
        Serial.println(command);
        dimmer.setState(ON);
    } else if (strcmp(command.c_str(), "off") == 0) {
        Serial.println(command);
        dimmer.setState(OFF);
    } else if (strcmp(command.c_str(), "high") == 0) {
        Serial.println(command);
        dimmer.setPower(100);
        Serial.println("Brightness set to 100%");
    } else if (strcmp(command.c_str(), "mid") == 0) {
        Serial.println(command);
        dimmer.setPower(75);
        Serial.println("Brightness set to 75%");
    } else if (strcmp(command.c_str(), "low") == 0) {
        Serial.println(command);
        dimmer.setPower(50);
        Serial.println("Brightness set to 50%");
    } else if (strcmp(command.c_str(), "dim") == 0) {
        Serial.println(command);
        dimmer.setPower(25);
        Serial.println("Brightness set to 25%");
    }
    previousCommand = command;
    Serial.println("----------------------------------------------------");
  }
}
 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("lightcontrol-001" )) {
       Serial.println("connected");  
     } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
 
  client.subscribe("lightcmd");
}
 
void loop() {
  client.loop();
}
