// GUIA BASE: https://randomnerdtutorials.com/esp32-https-requests-sim-card-sim7000g/
#define TINY_GSM_MODEM_SIM7000SSL
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#define SerialMon Serial
#define SerialAT Serial1

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#define TINY_GSM_DEBUG Serial
#include "utils.h"

// flag to force SSL client authentication, if needed
// #define TINY_GSM_SSL_CLIENT_AUTHENTICATION

// Server details
const char server[] = "gist.githubusercontent.com";
const char resource[] = "/RuiSantosdotme/7db8537cef1c84277c268c76a58d07ff/raw/d3fe4cd6eff1ed43e6dbd1883ab7eba8414e2406/gistfile1.txt";
const int port = 443;

TinyGsm modem(SerialAT);

TinyGsmClientSecure client(modem);
HttpClient http(client, server, port);

void setup() {

  SerialMon.begin(115200);

  initializeModem(modem);
  getModemGeneralInfo(modem);
  registerToNetwork(modem);
  connectToGPRS(modem);

  delay(1000);
}

void loop() {

  SerialMon.print(F("Performing HTTPS GET request... "));
  http.connectionKeepAlive(); // Currently, this is needed for HTTPS
  int err = http.get(resource);
  if (err != 0) {
    SerialMon.println(F("failed to connect"));
    delay(5000);
    return;
  }

  int status = http.responseStatusCode();
  SerialMon.print(F("Response status code: "));
  SerialMon.println(status);
  if (!status) {
    delay(5000);
    return;
  }

  SerialMon.println(F("Response Headers:"));
  while (http.headerAvailable()) {
    String headerName = http.readHeaderName();
    String headerValue = http.readHeaderValue();
    SerialMon.println("    " + headerName + " : " + headerValue);
  }

  int length = http.contentLength();
  if (length >= 0) {
    SerialMon.print(F("Content length is: "));
    SerialMon.println(length);
  }
  if (http.isResponseChunked()) {
    SerialMon.println(F("The response is chunked"));
  }

  String body = http.responseBody();
  SerialMon.println(F("Response:"));
  SerialMon.println(body);

  SerialMon.print(F("Body length is: "));
  SerialMon.println(body.length());

  // Shutdown
  http.stop();
  SerialMon.println(F("Server disconnected"));

  modem.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected"));

  // Do nothing forevermore
  while (true) {
    delay(1000);
  }
}