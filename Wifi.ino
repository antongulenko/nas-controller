#include <WiFi.h>
#include <WiFiClient.h>

// The following variables can be declared and set in a separate file
extern const char *wifiSsid;
extern const char *wifiPassword;

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPassword);
  Serial.print(F("Connecting to Wifi "));

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print(F("Connected to "));
  Serial.println(wifiSsid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}
