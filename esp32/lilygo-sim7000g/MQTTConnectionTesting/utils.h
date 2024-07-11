
#include <ArduinoJson.h>

// Your GPRS credentials
const char apn[] = "claro.pe";
const char gprsUser[] = "";
const char gprsPass[] = "";

// LilyGO T-SIM7000G Pinout
#define UART_BAUD 115200
#define PIN_TX 27
#define PIN_RX 26
#define GSM_PIN ""

void initializeModem(TinyGsm modem) {
  // Set modem baud rate and Pins
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(1000);

  SerialMon.println("Initializing modem...");
  if (!modem.restart()) {
    SerialMon.println("Failed to restart modem, attempting to continue without restarting");
  }
  delay(5000);

  // Unlock your SIM card with a PIN
  if (GSM_PIN && modem.getSimStatus() != 3) {
    modem.simUnlock(GSM_PIN);
  }
}

void getModemGeneralInfo(TinyGsm modem) {
  SerialMon.println("Showing general modem information: ");
  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

  String name = modem.getModemName();
  SerialMon.print("Modem Name: ");
  SerialMon.println(name);

  String ccid = modem.getSimCCID();
  SerialMon.print("CCID: ");
  SerialMon.println(ccid);

  String imei = modem.getIMEI();
  SerialMon.print("IMEI: ");
  SerialMon.println(imei);

  String imsi = modem.getIMSI();
  SerialMon.print("IMSI: ");
  SerialMon.println(imsi);

  String cop = modem.getOperator();
  SerialMon.print("Operator: ");
  SerialMon.println(cop);

  bool res = modem.isGprsConnected();
  SerialMon.print("GPRS status:: ");
  SerialMon.println(res ? "connected" : "not connected");
}


void registerToNetwork(TinyGsm modem) {
  if (modem.waitForNetwork()) {
    SerialMon.println("Registered on network.");
  } else {
    SerialMon.println("Not connected to the network. Attempting to connect...");
    registerToNetwork(modem);
  }
}

void connectToGPRS(TinyGsm modem) {
  if (modem.isGprsConnected()) {
    SerialMon.println("Connected to GPRS network.");
  } else {
    SerialMon.println("Connecting to the GPRS Network.");
    modem.gprsConnect(apn, gprsUser, gprsPass);
    delay(1000);
    connectToGPRS(modem);
  }
}


void jsonFormatter(char *jsonBuffer, String t, float h) {
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["humidity"] = h;
  doc["temperature"] = t;
  serializeJson(doc, jsonBuffer, 512);
  Serial.println(jsonBuffer);
}

void handleMessageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, length);
  Serial.println();
}
