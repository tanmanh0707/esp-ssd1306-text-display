#if defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <Huma_Buttons.h>
#include "common.h"
#include "web_index.h"

/* WIFI */
#if defined(ESP32)
#define WIFI_AP_SSID                  "ESP32 Access Point"
#elif defined(ESP8266)
#define WIFI_AP_SSID                  "ESP8266 Access Point"
#endif
#define WIFI_AP_PASSWORD              ""

/* Web Server */
#define WEBSERVER_PORT                80
#define SPIFFS_FILENAME_MSG           "textdisplay"

AsyncWebServer _server(WEBSERVER_PORT);
static String tempMsg = "";
static bool _updateText = false;

void WIFI_Init();
void handleSetMsg(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

void setup()
{
  Serial.begin(115200);
  SCREEN_Init();
  WIFI_Init();

  if (FS_Init())
  {
    File fd = FS_Open(SPIFFS_FILENAME_MSG, FS_OPEN_READ);
    if (fd) {
      size_t file_size = FS_FileSize(fd);
      char *buffer = (char*)calloc(1, file_size + 1);
      if (buffer) {
        FS_Read(fd, buffer, file_size);
        tempMsg = String(buffer);
        _updateText = true;
        Serial.printf("Read from FS: %s\n", tempMsg.c_str());
      } else { Serial.println("Allocate memory failed!"); }

      FS_Close(fd);
    } else { Serial.println("Open file failed!"); }
  }

  Serial.println("ESP SSD1306 Text Display Started!");

  if (! _updateText) {
    SCREEN_Display();
  }
}

void loop()
{
  if (_updateText)
  {
    _updateText = false;
    SCREEN_DrawMultiLineText(tempMsg);
  }
}

void WIFI_AccessPoint()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("WiFi Access Point Started! SSID: %s - Password: %s\n", WIFI_AP_SSID, WIFI_AP_PASSWORD);
}

void WIFI_Init()
{
  WIFI_AccessPoint();

  /* Homepage */
  _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html, sizeof(index_html));
//    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });
  
  /* Configure */
  _server.on("/setmsg", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200);
  }, NULL, handleSetMsg);

  /* Start server */
  _server.begin();
}

void handleSetMsg(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  if(!index) {
    Serial.printf("BodyStart: %u\n", total);
    tempMsg = "";
  }

  if (len) {
    char *buff = (char *)calloc(1, len + 1);
    if (buff) {
      memcpy(buff, data, len);
      tempMsg += String(buff);
      free(buff);
    }
  }
  
  if (index + len >= total) {
    Serial.printf("BodyEnd: %u - %s\n", total, tempMsg.c_str());
    File fd = FS_Open(SPIFFS_FILENAME_MSG, FS_OPEN_WRITE);
    if (fd) {
      FS_Write(fd, tempMsg.c_str(), tempMsg.length());
      FS_Close(fd);
    } else {
      Serial.println("Open file failed!");
    }
    _updateText = true;
  }
}
