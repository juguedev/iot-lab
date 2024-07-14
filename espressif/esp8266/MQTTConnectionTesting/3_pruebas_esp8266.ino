#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "utils.h"
#define TIME_ZONE -5
 
 // NODEMCU 1.0 Board
char publish_topic[15] = "esp8266/pub";
char subscribe_topic[15] = "esp8266/sub";
 
WiFiClientSecure net;
 
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
 
PubSubClient mqtt(net);
 
time_t now;
time_t nowish = 1510592825; 
char jsonBuffer[512];

void connectAWS()
{
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  mqtt.setServer(MQTT_HOST, 8883);
  mqtt.setCallback(handleMessageReceived);
 
 
  Serial.println("Connecting to AWS IOT");
  while (!mqtt.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!mqtt.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  mqtt.subscribe(subscribe_topic);
  Serial.println("AWS IoT Connected!");
}
 
 
void setup()
{
  Serial.begin(115200);
  connectToWifi();
  setSNTPSync(now, nowish);
  connectAWS();
}
 
 
void loop()
{
  delay(3000);
  now = time(nullptr);
 
  if (!mqtt.connected())
  {
    connectAWS();
  }
  else
  {
    mqtt.loop();
    jsonFormatter(jsonBuffer, "30", 10);
    mqtt.publish(publish_topic, jsonBuffer);
  }
}