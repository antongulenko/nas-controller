
#define PIN_ENCODER_CLK 5
#define PIN_ENCODER_DT 17
#define PIN_ENCODER_SW 16
#define PIN_BTN_BACK 18

#define BUTTON_PRESS_DURATION 50

int lastEncoderCounter = 0;
int encoderCounter = 0;
int currentEncoderClk;
int lastEncoderClk;

unsigned long encoderButtonDownStart = 0;
bool encoderButtonPressed = false;
bool encoderButtonHandled = false;

unsigned long backButtonDownStart = 0;
bool backButtonPressed = false;
bool backButtonHandled = false;

void setupInputs() {
  pinMode(PIN_ENCODER_CLK, INPUT);
  pinMode(PIN_ENCODER_DT, INPUT);
  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);
  pinMode(PIN_BTN_BACK, INPUT_PULLDOWN);
  lastEncoderClk = digitalRead(PIN_ENCODER_CLK);
}

void readInputs() {
  unsigned long now = millis();
  
  currentEncoderClk = digitalRead(PIN_ENCODER_CLK);
  if (currentEncoderClk != lastEncoderClk) {
    if (digitalRead(PIN_ENCODER_DT) == currentEncoderClk) {
      // Rotation counter clock-wise
      encoderCounter--;
    } else {
      // Rotation clock-wise
      encoderCounter++;      
    }
  }
  lastEncoderClk = currentEncoderClk;

  if (digitalRead(PIN_ENCODER_SW) == LOW) {
    if (encoderButtonDownStart == 0) {
      // First button down after being up
      encoderButtonDownStart = now;
    } else if(!encoderButtonHandled && now - encoderButtonDownStart > BUTTON_PRESS_DURATION) {
      encoderButtonPressed = true;
      encoderButtonHandled = true;
    }
  } else {
    encoderButtonDownStart = 0;
    encoderButtonHandled = false;
  }

  if (digitalRead(PIN_BTN_BACK) == HIGH) {
    if (backButtonDownStart == 0) {
      // First button down after being up
      backButtonDownStart = now;
    } else if(!backButtonHandled && now - backButtonDownStart > BUTTON_PRESS_DURATION) {
      backButtonPressed = true;
      backButtonHandled = true;
    }
  } else {
    backButtonDownStart = 0;
    backButtonHandled = false;
  }
}

bool wasEncoderButtonPressed() {
  bool result = encoderButtonPressed;
  encoderButtonPressed = false;
  return result;
}

bool wasBackButtonPressed() {
  bool result = backButtonPressed;
  backButtonPressed = false;
  return result;
}

int getEncoderMovement() {
  int result = encoderCounter - lastEncoderCounter;
  lastEncoderCounter = encoderCounter;
  return result;
}
