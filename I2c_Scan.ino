
void setupI2cScan() {
  Wire.begin(I2C_PIN_SDA, I2C_PIN_SCL);
}

void i2cScanLoop() {
  Serial.println("\nI2C Scanner started...");
 
  int nDevices = 0;
  for(byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
 
    if (error == 0) {
      Serial.printf("I2C device found at address %#04x\n", address);
      nDevices++;
    } else if (error == 4) {
      Serial.printf("Unknown error at address %#04x\n", address);
    }
  }
  Serial.printf("Scan done, devices found: %d\n", nDevices);
  delay(5000);
}
