#include "BluetoothSerial.h"

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

void registerToNetwork(TinyGsm modem) {
  delay(1000);
  if (modem.waitForNetwork()) {
    SerialMon.println("Registered on network.");
  } else {
    SerialMon.println("Not connected to the network. Attempting to connect...");
    registerToNetwork(modem);
  }
}

void connectToGPRS(TinyGsm modem) {
  delay(1000);
  if (modem.isGprsConnected()) {
    SerialMon.println("Connected to GPRS network.");
  } else {
    SerialMon.println("Connecting to the GPRS Network.");
    modem.gprsConnect(apn, gprsUser, gprsPass);
    delay(1000);
    connectToGPRS(modem);
  }
}


void  connectToBluetoothDevice() {
  SerialMon.println("Connecting to ELM327 - Phase 1: Bluetooth");

  if (!SerialELM.connect(address)) {  // Bu default #define READY_TIMEOUT (10 * 1000)
    SerialMon.println("Couldn't connect to ELM327 - Phase 1: Bluetooth");
    SerialMon.println("Retryng...");
    delay(2000);
    connectToBluetoothDevice();
  } else {
    SerialMon.println("Connected to ELM327 - Phase 1: Bluetooth");
  }
}


void handleMessageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, length);
  Serial.println();
}

void establishOBDConnection(bool debug, char protocol) {
  SerialMon.println("Connecting to ELM327 - Phase 2: OBD");
  if (!myELM327.begin(SerialELM, debug, 2000, protocol, 40, 0)) {
    SerialMon.println("Couldn't connect to OBD scanner - Phase 2: OBD");
    SerialMon.println("Retryng...");
    delay(2000);
    establishOBDConnection(debug, protocol);
  } else {
    SerialMon.println("Connected to ELM327 - Phase 2: OBD");
  }
  SerialMon.println("Connection process to ELM327 succesfully ended.");
}