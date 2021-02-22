
bool ledsEnabled = false;
byte prevBrightness = 0;

void handleMenu() {  
// TODO
// when button pressed, show info on: NAS on/off state, RGB Fan state, IP
// access to configuration: turn NAS on/off, configure RGB (on/off, colors, patterns)
  
//  wasEncoderButtonPressed()
//  wasBackButtonPressed()
//  getEncoderMovement()

  bool before = ledsEnabled;
  if (wasEncoderButtonPressed()) ledsEnabled = true;
  if (wasBackButtonPressed()) ledsEnabled = false;

  if (before != ledsEnabled)
    Serial.printf("LEDs now enabled: %d\n", ledsEnabled);

  if (ledsEnabled) {
    byte brightness = encoderCounter % 255;
    if (brightness != prevBrightness) {
      setFanColor(FAN1, brightness, brightness, brightness);
      setFanColor(FAN2, brightness, brightness, brightness);
      setFanColor(FAN3, brightness, brightness, brightness);
      prevBrightness = brightness;
    }
  } else {
    prevBrightness = 0;
    disableFanColor(FAN1);
    disableFanColor(FAN2);
    disableFanColor(FAN3);
  }
}
