#include "BluetoothSerial.h"
#include "ELMduino.h"


BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial

char protocol = ISO_15765_11_BIT_500_KBAUD;

//String MACadd = "DC:0D:30:48:8D:60";  00:10:cc:4f:36:03    //enter the ELM327 MAC address
uint8_t address[6]  = {0x00, 0x10, 0xCC, 0x4F, 0x36, 0x03};  //enter the ELM327 MAC address after the 0x


ELM327 myELM327;

uint32_t rpm = 0;


void setup()
{
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  DEBUG_PORT.begin(38400);
  //SerialBT.setPin("1234");
  ELM_PORT.begin("ESP32", true);

  if (!ELM_PORT.connect(address))            //"OBDII" replaced by "address"
  {
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
    while (1);
  }

  if (!myELM327.begin(ELM_PORT, protocol='A'))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");
}


void loop()
{
  float tempRPM = myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(rpm);
  }
  else
  {
    printError();
  }
}


void printError()
{
  Serial.print("Received: ");
  for (byte i = 0; i < myELM327.recBytes; i++)
    Serial.write(myELM327.payload[i]);
  Serial.println();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
    Serial.println(F("\tELM_SUCCESS"));
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();

  delay(100);
}