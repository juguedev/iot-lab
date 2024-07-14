#include "BluetoothSerial.h"
#include "ELMduino.h"

BluetoothSerial SerialBT;
#define SerialELM SerialBT
#define SerialMon Serial

char protocol = ISO_15765_11_BIT_500_KBAUD;

//String MACadd = "DC:0D:30:48:8D:60";  00:10:cc:4f:36:03    //enter the ELM327 MAC address
uint8_t address[6] = { 0x00, 0x10, 0xCC, 0x4F, 0x36, 0x03 };  //enter the ELM327 MAC address after the 0x
ELM327 myELM327;

#include "utils.h"


void setup() {
  SerialMon.begin(115200);
  SerialELM.begin("ESP32", true);
  //SerialBT.setPin("1234");

  connectToBluetoothDevice();
  establishOBDConnection();
}


void loop() {
  float tempRPM = (uint32_t)myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS) {
    Serial.print("RPM: "); Serial.println(tempRPM);
  } else {
    Serial.print("ERROR: ");
    for (byte i = 0; i < myELM327.recBytes; i++)
      Serial.write(myELM327.payload[i]);
    Serial.println();
  }
  delay(1000);
}
