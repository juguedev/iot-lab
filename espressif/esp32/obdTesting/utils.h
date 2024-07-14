// This should be declared after the declaration of SerialMon, myELM327 and the other needed objects
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

void establishOBDConnection() {
  SerialMon.println("Connecting to ELM327 - Phase 2: OBD");
  if (!myELM327.begin(SerialELM, protocol = 'A')) {
    SerialMon.println("Couldn't connect to OBD scanner - Phase 2: OBD");
    SerialMon.println("Retryng...");
    delay(2000);
    establishOBDConnection();
  } else {
    SerialMon.println("Connected to ELM327 - Phase 2: OBD");
  }
  SerialMon.println("Connection process to ELM327 succesfully ended.");
}