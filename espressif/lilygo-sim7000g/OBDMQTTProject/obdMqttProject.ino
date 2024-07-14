#define TINY_GSM_MODEM_SIM7000
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SSLClient.h>
#include <SoftwareSerial.h>
#include "BluetoothSerial.h"
#include "ELMduino.h"

BluetoothSerial SerialBT;
#define SerialELM SerialBT
#define SerialMon Serial
#define SerialAT Serial1

bool debug = false; 
char protocol = '1'; 

//String MACadd = "DC:0D:30:48:8D:60";  00:10:cc:4f:36:03    //enter the ELM327 MAC address
uint8_t address[6] = { 0x00, 0x10, 0xCC, 0x4F, 0x36, 0x03 };  //enter the ELM327 MAC address after the 0x
ELM327 myELM327;


// Your AWS IoT endpoint
const char* mqtt_server = "a1gftcdeurf2r1-ats.iot.us-east-1.amazonaws.com";
const char* thing = "esp_8266";
const int mqtt_port = 8883;

char publish_topic[15] = "esp8266/pub";
char subscribe_topic[15] = "esp8266/sub";

char jsonBuffer[512];

TinyGsm modem(SerialAT);
TinyGsmClient base_client(modem);



#include "utils.h"

void setup() {
  SerialMon.begin(115200);


  initializeModem(modem);
  registerToNetwork(modem);
  connectToGPRS(modem);

  SerialELM.begin("ESP32", true);
  //SerialBT.setPin("1234");

  connectToBluetoothDevice();
  establishOBDConnection(debug, protocol);
}


void loop() {
  float tempRPM = (uint32_t)myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    Serial.print("RPM: "); Serial.println(tempRPM);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();
  
}
