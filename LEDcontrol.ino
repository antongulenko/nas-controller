
const byte ledModeOff = 0;
const byte ledModeConstant = 1;

byte ledMode;

byte rgb1[3];
byte rgb2[3];
byte rgb3[3];

static void _applyModeConstant() {
  setFanColor(FAN1, rgb1[0], rgb1[1], rgb1[2]);
  setFanColor(FAN2, rgb2[0], rgb2[1], rgb2[2]);
  setFanColor(FAN3, rgb3[0], rgb3[1], rgb3[2]);
}

void setLedMode(byte mode) {
  ledMode = mode;
  switch (ledMode) {
    case ledModeOff:
      disableFanColor(FAN1);
      disableFanColor(FAN2);
      disableFanColor(FAN3);
      break;
    case ledModeConstant:
      _applyModeConstant();
      break;
  }
}

void turnLedsOff() {
  setLedMode(ledModeOff);
}

void setConstantColors(bool overwrite[9], byte newValues[9]) {
  if (overwrite[0]) rgb1[0] = newValues[0];
  if (overwrite[1]) rgb1[1] = newValues[1];
  if (overwrite[2]) rgb1[2] = newValues[2];
  if (overwrite[3]) rgb2[0] = newValues[3];
  if (overwrite[4]) rgb2[1] = newValues[4];
  if (overwrite[5]) rgb2[2] = newValues[5];
  if (overwrite[6]) rgb3[0] = newValues[6];
  if (overwrite[7]) rgb3[1] = newValues[7];
  if (overwrite[8]) rgb3[2] = newValues[8];
  setLedMode(ledModeConstant);
}

String currentLedMode() {
  String res = "";
  switch (ledMode) {
    case ledModeOff:
      res += "LEDs off";
      break;
    case ledModeConstant:
      res += "LEDs constant (";
      res += "RGB fan 1: ";
      res += rgb1[0];
      res += " ";
      res += rgb1[1];
      res += " ";
      res += rgb1[2];
      res += ", ";
      res += "RGB fan 2: ";
      res += rgb2[0];
      res += " ";
      res += rgb2[1];
      res += " ";
      res += rgb2[2];
      res += ", ";
      res += "RGB fan 3: ";
      res += rgb3[0];
      res += " ";
      res += rgb3[1];
      res += " ";
      res += rgb3[2];
      res += ")";
      break;
    default:
      res += "Unknown mode (";
      res += ledMode;
      res += ")";
      break;
  }
  return res;
}
