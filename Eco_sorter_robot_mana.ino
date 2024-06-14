#include "WiFiS3.h"
#include <Servo.h>
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>

Servo baza;
Servo jos;
Servo mid;
Servo sus;
Servo grip;
int ok;
char ssid[] = "ok";    // your network SSID (name)
char pass[] = "sajp1434";    // your network password 

char mqtt_user[] = "Eco";
char mqtt_pass[] = "Sorter";
const char* host = "192.168.171.226";         // server address
const int httpPort = 5000;                  // server port
 String line;
 
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char subscribe_topic[]  = "/brat";
const char publish_topic[]  = "/hello/world";
 void setup() {
    Serial.begin(115200);
  // put your setup code here, to run once:
baza.attach(5);
jos.attach(9);
mid.attach(7);
sus.attach(6);
grip.attach(8);
  Serial.print("Connecting to ");
  Serial.println(ssid);

    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("You're connected to the network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  mqttClient.setUsernamePassword(mqtt_user, mqtt_pass);

  Serial.print("Attempting to connect to the MQTT broker.");

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
    Serial.println("You're connected to the MQTT broker!");

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(subscribe_topic);

  // subscribe to a topic
  mqttClient.subscribe(subscribe_topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(subscribe_topic);

}

void loop() {


  mqttClient.poll();

  // send message, the Print interface can be used to set the message contents
  delay(3000);

}

String conn(String path){

  WiFiClient client;
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    Serial.println(WiFi.status());

  }

  // We now create a URI for the request
  String url = path;

  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "err";
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    line = client.readStringUntil('\r');
    
  }
return line;
}



void onMqttMessage(int messageSize) {
  Serial.print("Received a message with topic '");
  Serial.println(mqttClient.messageTopic());
  StaticJsonDocument<256> doc;
  deserializeJson(doc, mqttClient);
  const char* message = doc["message"];
  Serial.print("Message: '");
  Serial.println(message);
  Serial.println((int)message);

  if((int)message == 536880008){

Serial.println("OK");
  grip.write(180);
  delay(1000);
  mid.write(135);
  delay(1000);
  
  delay(1000);
  mid.write(90);
  delay(1000);
  baza.write(0);
  delay(1000);
  mid.write(135);
  delay(1000);
  grip.write(90);
  delay(2500);
  mid.write(90);
  delay(1000);
  baza.write(90);
  delay(1000);

delay(4000);

  }
}