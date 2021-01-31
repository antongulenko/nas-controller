#include <ESPmDNS.h>
#include <Wire.h>
#include <HTTP_Method.h>
#include <WebServer.h>

void connectWifi();
void setupWebServer();
void loopWebServer();

#define I2C_PIN_SCL 4
#define I2C_PIN_SDA 0
#define I2C_FREQ 100000
#define ONBOARD_LED 22

TwoWire I2C = TwoWire(0);

void onboardLed(bool on) {
  if (on) {
    digitalWrite(ONBOARD_LED, 0);
  } else {
    digitalWrite(ONBOARD_LED, 1);
  }
}

void setupMain() {
  setupInputs();
  pinMode(ONBOARD_LED, OUTPUT);
  onboardLed(false);
  
  if (I2C.begin(I2C_PIN_SDA, I2C_PIN_SCL, 100000)) {
    Serial.printf("I2C bus initialized on pins SCL %d and SDA %d, frequency %d\n", I2C_PIN_SCL, I2C_PIN_SDA, I2C_FREQ);
  } else {
    Serial.println("Failed to initialize I2C bus!");
  }

  setupScreen();
  setupLeds();
  setupPower();
  connectWifi();
  
  if (MDNS.begin("esp32")) {
    Serial.println(F("MDNS responder started"));
  }
  
  setupWebServer();
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("\nHello!"));
  
  setupMain();
// setupI2cScan();
}

void loop() {
  readInputs();
// handleMenu();
  loopWebServer();
// i2cScanLoop();
}
