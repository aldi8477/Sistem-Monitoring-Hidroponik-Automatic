#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <StringSplitter.h>

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial mySerial(D8, D7);

#define LED1 LED_BUILTIN
#define LED2 D4
#define LED3 D5
#define LED4 D6

#define LED_ON HIGH
#define LED_OFF LOW

#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11

static String 
  DEVICE_ID     = "HIDROPONIK_POLINDRA",
  WIFI_SSID     = "HUAWEI-385B",
  WIFI_PASS     = "29207350", 
  MQTT_HOST     = "broker.hivemq.com",
  TOPIC_PUB     = "hidroponik_polindra_data",
  TOPIC_SUB     = "hidroponik_polindra_aksi";

static int MQTT_PORT = 1883;

DHT dht(DHTPIN, DHTTYPE);

float h, t;

String data;
String buffData;
String buff[4];

char pus[50];

void setupWifi(String ssid, String password) {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT setup : ");
    Serial.print(MQTT_HOST);
    if (client.connect(DEVICE_ID.c_str())) {
      Serial.println("....Connected");
      client.subscribe(TOPIC_SUB.c_str());
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  buffData = "";
  for(int i=0; i<length; i++){
    buffData += (char) payload[i];
  }
  
  Serial.print(topic);
  Serial.print(" ==> ");
  Serial.println(buffData);

  if(buffData == "R1_ON") {
    digitalWrite(LED_BUILTIN, LED_ON);
  }else if(buffData == "R2_ON") {
    digitalWrite(LED2, LED_ON);
  }else if(buffData == "R3_ON") {
    digitalWrite(LED3, LED_ON);
  }else if(buffData == "R4_ON") {
    digitalWrite(LED4, LED_ON);
  }else if(buffData == "R1_OFF") {
    digitalWrite(LED_BUILTIN, LED_OFF);
  }else if(buffData == "R2_OFF") {
    digitalWrite(LED2, LED_OFF);
  }else if(buffData == "R3_OFF") {
    digitalWrite(LED3, LED_OFF);
  }else if(buffData == "R4_OFF") {
    digitalWrite(LED4, LED_OFF);
  }
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  dht.begin();
  setupWifi(WIFI_SSID, WIFI_PASS);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  client.setServer(MQTT_HOST.c_str(), MQTT_PORT);
  client.setCallback(callback);
}

unsigned long waiting = millis();
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  

  if(mySerial.available() > 0) {
    String incomingByte=mySerial.readStringUntil('\n');
    StringSplitter *splitter = new StringSplitter(incomingByte, ',', 3);  // new StringSplitter(string_to_split, delimiter, limit)
    int itemCount = splitter->getItemCount();

    for(int i = 0; i < itemCount; i++){ //String item = splitter->getItemAtIndex(i);
      buff[i] = splitter->getItemAtIndex(i);
      
    }

    Serial.println(buff[0]);

    sprintf(pus,"%s",buff[0]);
    Serial.println(pus);
    client.publish(TOPIC_PUB.c_str(), pus); 
  }

}
