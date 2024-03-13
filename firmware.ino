#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "RBDdimmer.h"

#define zerocross D6
#define outputPin D7


const char *ssid =  "Tim";    
const char *pass =  "7e7ce00618a5";
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
dimmerLamp dimmer(outputPin, zerocross);


void mqttReconnect() {
  while (!client.connected()) {
   Serial.println("Connected to the WiFi network");
   client.setServer(mqttServer, mqttPort);
   while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
     if (client.connect("parking-device-01")) {
       Serial.println("connected");
     } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  }
}


void setup() {
 Serial.begin(115200);
 dimmer.begin(NORMAL_MODE, ON);
 Serial.println("Starting...");
  delay(1000);
  
  WiFi.begin(ssid, pass);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to the WiFi network");
  mqttReconnect();
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String stMessage;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "lightcmd") {
    Serial.print("Changing output to ");
    if(stMessage == "on"){
      Serial.println("on");
       dimmer.setState(ON);
    }
    else if(stMessage == "off"){
      Serial.println("off");
      dimmer.setState(OFF);
    }
    else if(stMessage == "high"){
      Serial.println("high");
      dimmer.setPower(100);
    }
    else if(stMessage == "mid"){
      Serial.println("mid");
      dimmer.setPower(75);
    }
    else if(stMessage == "low"){
      Serial.println("low");
      dimmer.setPower(50);
    }
    else if(stMessage == "dim"){
      Serial.println("dim");
      dimmer.setPower(25);
    }
  }
}

void loop() {
 client.loop();
 delay(100);
}
