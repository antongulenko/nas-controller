#include <sstream>
#include <string>
#include <stdlib.h>

WebServer server(80);

const char *templateStatus = 1 + R"TEMPLATE(
<html>
  <head>
    <meta http-equiv='refresh' content='5'/>
    <title>Status</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
    </style>
  </head>
  <body>
    <h1>Hello from NAS controller!</h1>
    <p>Uptime: %02d:%02d:%02d</p>
    <p>IP: %s</p>
    <p>NAS power: %s</p>
    <p>Encoder counter: %d, moved: %d</p>
    <p>LED mode: %s</p>
  </body>
</html>
)TEMPLATE";

void loopWebServer() {
  server.handleClient();
}

static void http(const String &uri, HTTPMethod method, WebServer::THandlerFunction fn) {
  server.on(uri, method, [fn]() {
    onboardLed(true);  
    fn();
    onboardLed(false);
  });
}

static void get(const String &uri, WebServer::THandlerFunction fn) {
  http(uri, HTTP_GET, fn);
}

static void post(const String &uri, WebServer::THandlerFunction fn) {
  http(uri, HTTP_POST, fn);
}

static void handleNotFound() {
  onboardLed(true);

  String resp = "";
  resp += "File not found\n\n";
  resp += "URI: " + server.uri() + "\n";
  resp += String("Method: ") + ((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
  resp += String("Headers (") + server.headers() + "):\n";
  for (uint8_t i = 0; i < server.headers(); i++) {
    resp += String("  ") + server.headerName(i) + ": " + server.header(i) + "\n";
  }  
  resp += String("Arguments (") + server.args() + "):\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    resp += "  " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  resp += "\n";

  server.send(404, "text/plain", resp);
  onboardLed(false);
}

static void handleStatus() {
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  String ip = WiFi.localIP().toString();
  String powerState = isPoweredOn() ? "on" : "off";
  
  char resp[512];
  snprintf(resp, sizeof(resp), templateStatus,
    hr, min % 60, sec % 60,
    ip.c_str(), powerState, encoderCounter, getEncoderMovement(),
    currentLedMode().c_str());
  server.send(200, "text/html", resp);
}

static void handlePulsePower() {
  int msec = 200;
  if (server.hasArg("msec")) {
    String msecStr = server.arg("msec");
    msec = strtoul(msecStr.c_str(), NULL, 0);
    if (msec <= 0 || msec >= 20000) {
      char resp[100];
      snprintf(resp, sizeof(resp), "Argument 'msec' must be unsigned long between 1 and 20000. Param string: '%s', parsed value: %d.", msecStr, msec);
      server.send(500, "text/plain", resp);
      return;
    }
  }

  pulsePowerButton(msec);
  
  char resp[100];
  snprintf(resp, sizeof(resp), "Power button pressed for %d ms\n", msec);
  server.send(200, "text/plain", resp);
}

void handleLedsOff() {
  turnLedsOff();
  server.send(200, "text/plain", "Leds turned off\n");
}

void handleLedsOn() {
  bool overwrite[9] = {false, false, false, false, false, false, false, false, false};
  byte values[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  if (server.hasArg("reset")) {
    for (int i = 0; i < 9; i++) overwrite[i] = true;
  }
  const String argNames[9] = {"r1", "g1", "b1", "r2", "g2", "b2", "r3", "g3", "b3"};
  for (int i = 0; i < 9; i++) {
    String argName = argNames[i];
    if (server.hasArg(argName)) {
      String colorStr = server.arg(argName);
      values[i] = (byte) strtoul(colorStr.c_str(), NULL, 0);
      overwrite[i] = true;
    }
  }
  
  setConstantColors(overwrite, values);
  server.send(200, "text/plain", "Leds status: " + currentLedMode() + "\n");
}

void setupWebServer() {
  get("/status", handleStatus);
  post("/powerButton", handlePulsePower);
  post("/leds/on", handleLedsOn);
  post("/leds/off", handleLedsOff);
  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
