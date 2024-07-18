#define TINY_GSM_MODEM_SIM7000
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <SSLClient.h>
#include <SoftwareSerial.h>
#include "BluetoothSerial.h"
#include "ELMduino.h"

// FUNCIONAAAA PERO CON LOS SIGUIENTES PROBLEMAS DETECTADOS
// Cada cierto tiempo el modulo pierde conectividad (se bugea) y necesito cargar el de sms
// Cada cierto tiempo el modulo cambia la medición de RPMs (Revisando logs al desconectarse el ssl, los datos comienzan a tener sentido)
// Cada cierto tiempo el modulo deja de pasar la segunda Phase (De momento hemos detectado que al desactivar el verbose) pasa
// Faltan añadir validaciones
// Faltan añadir el bucle del connect IoT

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
SSLClient ssl_client(&base_client);
PubSubClient mqtt(ssl_client);

// IT IS EXTREMELY IMPORTANT TO INCLUDE UTILS AFTER DEFINING THE COMPONENTS IT REQUIRES
#include "secrets.h"
#include "utils.h"


// Connect to AWS IoT
void connectAWSIoT() {
  ssl_client.setCACert(ca_cert);
  ssl_client.setCertificate(client_cert);
  ssl_client.setPrivateKey(client_key);

  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(handleMessageReceived);
  
  SerialMon.println("Connecting to AWS IoT...");
  mqtt.connect(thing);
}

void setup() {
  SerialMon.begin(115200);

  // The following lines are needed to start the modem and avoid connectivity loss
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(300);
  digitalWrite(4, LOW);

  initializeModem(modem);
  registerToNetwork(modem);
  connectToGPRS(modem);
  connectAWSIoT();

  // IT IS EXTREMELY IMPORTANT TO SET UP THE DEVICE GPRS BEFORE STARTING THE ELM CONNECTION
  SerialELM.begin("ESP32", true);
  //SerialBT.setPin("1234");

  connectToBluetoothDevice();
  establishOBDConnection(debug, protocol);
}


void loop() {
  float tempRPM = (uint32_t)myELM327.rpm();
  mqtt.loop();
  
  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    Serial.print("RPM: "); Serial.println(tempRPM);
    jsonFormatter(jsonBuffer, "30", tempRPM);
    mqtt.publish(publish_topic, jsonBuffer);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();
  delay(500);
}
