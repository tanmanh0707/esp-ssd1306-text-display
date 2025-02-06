#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "common.h"

#define SCREEN_WIDTH            128
#define SCREEN_HEIGHT           64
#define OLED_RESET              -1
#define SCREEN_ADDRESS          0x3C

/* ESP32-C3 I2C default pins
#define OLED_SDA_PIN            8
#define OLED_SCL_PIN            9
*/

Adafruit_SSD1306 _oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool SCREEN_Init()
{
  bool ret = _oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if( ! ret) {
    Serial.println(F("SSD1306 begin failed"));
  } else {
    _oled.clearDisplay();
  }
  return ret;
}
