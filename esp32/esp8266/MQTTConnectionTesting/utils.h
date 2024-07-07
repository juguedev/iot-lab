#include <ArduinoJson.h>
#include <PubSubClient.h>

void handleMessageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, length);
  Serial.println();
}
 
void connectToWifi()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
}

void setSNTPSync(time_t now, time_t nowish)
{
  // The certificate validity and TLS handshake need the current time from the SNTP server. So, the SNTP server should be enabled.
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\nSNTP is configured for TLS connections.");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

 
void jsonFormatter(char* jsonBuffer, String t, float h)
{
  StaticJsonDocument<200> doc;
      doc["time"] = millis();
      doc["humidity"] = h;
      doc["temperature"] = t;
  serializeJson(doc, jsonBuffer, 512);
  Serial.println(jsonBuffer);
}
