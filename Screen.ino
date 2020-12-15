#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display;

void setupScreen() {
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C, -1);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 screen setup failed"));
    return;
  }
  
  Serial.println("Showing splash screen...");
  display.display(); // Show splash screen
  
  // Default text settings
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
}
