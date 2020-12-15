
#define PIN_POWER_IN 2
#define PIN_POWER_OUT 15

void setupPower() {
  pinMode(PIN_POWER_IN, INPUT);
  pinMode(PIN_POWER_OUT, OUTPUT);
  digitalWrite(PIN_POWER_OUT, LOW);
}

bool isPoweredOn() {
  return digitalRead(PIN_POWER_IN) == HIGH;
}

void pulsePowerButton(unsigned long milliseconds) {
  digitalWrite(PIN_POWER_OUT, HIGH);
  delay(milliseconds);
  digitalWrite(PIN_POWER_OUT, LOW);
}
