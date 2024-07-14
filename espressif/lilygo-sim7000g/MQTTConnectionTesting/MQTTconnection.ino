#define TINY_GSM_MODEM_SIM7000
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SSLClient.h>
#include <SoftwareSerial.h>

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define your serial connections
#define SerialMon Serial
#define SerialAT Serial1

#include "secrets.h"
#include "utils.h"

// Your AWS IoT endpoint
const char* mqtt_server = "a1gftcdeurf2r1-ats.iot.us-east-1.amazonaws.com";
const char* thing = "esp_8266";
const int mqtt_port = 8883;

char publish_topic[15] = "esp8266/pub";
char subscribe_topic[15] = "esp8266/sub";

char jsonBuffer[512];

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient base_client(modem);
SSLClient ssl_client(&base_client);
PubSubClient mqtt(ssl_client);


// Connect to AWS IoT
void connectAWSIoT() {
  ssl_client.setCACert(ca_cert);
  ssl_client.setCertificate(client_cert);
  ssl_client.setPrivateKey(client_key);

  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(handleMessageReceived);


  while (!mqtt.connected()) {
    SerialMon.println("Connecting to AWS IoT...");
    if (mqtt.connect(thing)) {
      mqtt.subscribe(subscribe_topic);
      Serial.println("AWS IoT Connected!");
    } else {
      SerialMon.print("failed, rc=");
      SerialMon.println(mqtt.state());
      SerialMon.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  SerialMon.begin(115200);

  initializeModem(modem);
  // getModemGeneralInfo(modem);
  registerToNetwork(modem);
  connectToGPRS(modem);
  connectAWSIoT();
}

void loop() {

  delay(5000);  // Send a message every 10 seconds

  if (!mqtt.connected()) {
    connectAWSIoT();
  } else {
    mqtt.loop();
    jsonFormatter(jsonBuffer, "30", 10);
    mqtt.publish(publish_topic, jsonBuffer);
  }
}
