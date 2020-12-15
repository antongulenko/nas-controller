
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

#define FAN1 1
#define FAN2 2
#define FAN3 3

#define LED_1_R 33
#define LED_1_G 25
#define LED_1_B 32
#define LED_2_R 27
#define LED_2_G 14
#define LED_2_B 26
#define LED_3_R 34
#define LED_3_G 35
#define LED_3_B 12

#define CHAN_1_R 0
#define CHAN_1_G 1
#define CHAN_1_B 2
#define CHAN_2_R 3
#define CHAN_2_G 4
#define CHAN_2_B 5
#define CHAN_3_R 6
#define CHAN_3_G 7
#define CHAN_3_B 8

void setupLedChannel(int chan, int pin) {
  ledcSetup(chan, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(pin, chan);
}

void setupLeds() {
  setupLedChannel(CHAN_1_R, LED_1_R);
  setupLedChannel(CHAN_1_G, LED_1_G);
  setupLedChannel(CHAN_1_B, LED_1_B);
  setupLedChannel(CHAN_2_R, LED_2_R);
  setupLedChannel(CHAN_2_G, LED_2_G);
  setupLedChannel(CHAN_2_B, LED_2_B);
  setupLedChannel(CHAN_3_R, LED_3_R);
  setupLedChannel(CHAN_3_G, LED_3_G);
  setupLedChannel(CHAN_3_B, LED_3_B);
}

void disableFanColor(byte fan) {
  switch (fan) {
    case FAN1:
      ledcWrite(CHAN_1_R, 0);
      ledcWrite(CHAN_1_G, 0);
      ledcWrite(CHAN_1_B, 0);
      break;
    case FAN2:
      ledcWrite(CHAN_2_R, 0);
      ledcWrite(CHAN_2_G, 0);
      ledcWrite(CHAN_2_B, 0);
      break;
    case FAN3:
      ledcWrite(CHAN_3_R, 0);
      ledcWrite(CHAN_3_G, 0);
      ledcWrite(CHAN_3_B, 0);
      break;
  }
}

void setFanColor(byte fan, byte r, byte g, byte b) {
  // Serial.printf("Setting PWMs for fan %d to: R = %#04x, G = %#04x, B = %#04x\n", fan, colors[0], colors[1], colors[2]);
  switch (fan) {
    case FAN1:
      ledcWrite(CHAN_1_R, r);
      ledcWrite(CHAN_1_G, g);
      ledcWrite(CHAN_1_B, b);
      break;
    case FAN2:
      ledcWrite(CHAN_2_R, r);
      ledcWrite(CHAN_2_G, g);
      ledcWrite(CHAN_2_B, b);
      break;
    case FAN3:
      ledcWrite(CHAN_3_R, r);
      ledcWrite(CHAN_3_G, g);
      ledcWrite(CHAN_3_B, b);
      break;
  }
}

// Source: https://gist.github.com/postspectacular/2a4a8db092011c6743a7
// HSV->RGB conversion based on GLSL version
// expects hsv channels defined in 0.0 .. 1.0 interval
float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float* hsv2rgb(float h, float s, float v, float* rgb) {
  rgb[0] = v * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = v * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = v * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}

float* rgb2hsv(float r, float g, float b, float* hsv) {
  float s = step(b, g);
  float px = mix(b, g, s);
  float py = mix(g, b, s);
  float pz = mix(-1.0, 0.0, s);
  float pw = mix(0.6666666, -0.3333333, s);
  s = step(px, r);
  float qx = mix(px, r, s);
  float qz = mix(pw, pz, s);
  float qw = mix(r, px, s);
  float d = qx - min(qw, py);
  hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
  hsv[1] = d / (qx + 1e-10);
  hsv[2] = qx;
  return hsv;
}
