#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include "custom_fonts.h"
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
    Serial.println(F("SSD1306 begin failed!"));
  } else {
    _oled.setFont(&__Anonymous5pt7b);
    _oled.setTextSize(1);
    _oled.setTextColor(SSD1306_WHITE);
    _oled.cp437(true);
    Serial.println(F("SSD1306 begin done!"));
  }

// Testing
  String test = "Lorem ipsum dolor sit";// dolor sit amet, consectetur adipiscing elit.";
  int16_t x1 = 0, y1 = 0;
  uint16_t w = 0, h = 0;
  _oled.getTextBounds(test, 0, 0, &x1, &y1, &w, &h);
  Serial.printf("x1: %d, y1: %d, w: %d, h: %d\n", x1, y1, w, h);
//
  
  return ret;
}

void SCREEN_Display()
{
  _oled.display();
}

void SCREEN_Clear()
{
  _oled.clearDisplay();
}

void SCREEN_SplitTextByLines(String &text, std::vector<String> &line_list, bool append /* = false */)
{
  int16_t x1 = 0, y1 = 0;
  uint16_t w = 0, h = 0, text_h = 0;
  int len = text.length();
  String line = "", tmp_word = "";

  if (false == append) {
    line_list.clear();
  }

  _oled.getTextBounds("My", 0, 0, &x1, &y1, &w, &text_h);
  Serial.printf("Text_h: %d\n", text_h);

  for (int i = 0; i < len; i++) {
    if (text[i] != ' ') {
      tmp_word += text[i];
    } else {
      _oled.getTextBounds(line + tmp_word, 0, 0, &x1, &y1, &w, &h);
      if (h <= text_h) {
        line += tmp_word + text[i];
        tmp_word = "";
        continue;
      } else {
        line_list.push_back(line);
        line = tmp_word + text[i];
        tmp_word = "";
      }
    }
  }
}

void SCREEN_DrawTextByLine(std::vector<String> &lines, uint32_t start_line /* = 0 */)
{
  int16_t x1 = 0, y1 = 0;
  uint16_t w = 0, h = 0, start_y = 0, draw_y = 0;

  _oled.getTextBounds("My", 0, 0, &x1, &y1, &w, &start_y);
  draw_y = start_y;

  for (int i = start_line; i < lines.size(); i++) {
    _oled.setCursor(0, draw_y);
    _oled.print(lines[i]);
    draw_y += start_y + 2;
    if (start_y > SCREEN_HEIGHT - start_y) {
      break;
    }
  }
  _oled.display();
}

void SCREEN_DrawMultiLineText(String &text, uint32_t start_line /* = 0 */)
{
#if 1
  std::vector<String> lines;
  SCREEN_SplitTextByLines(text, lines, false);
  _oled.clearDisplay();
  SCREEN_DrawTextByLine(lines, start_line);
#else
  _oled.clearDisplay();
  _oled.setCursor(0, 11);
  _oled.print(text);
  _oled.display();
#endif
}
