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

AsyncWebServer _server(WEBSERVER_PORT);

void WIFI_Init();
void handleSetMsg(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

void setup()
{
  Serial.begin(115200);
  SCREEN_Init();
  WIFI_Init();
  Serial.printf("ESP SSD1306 Text Display Started!");
}

void loop()
{
  
}

void WIFI_AccessPoint()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("WiFi Access Point started! SSID: %s - Password: %s\n", WIFI_AP_SSID, WIFI_AP_PASSWORD);
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
    Serial.printf("BodyStart: %u", total);
//    request->_tempFile = SPIFFS.open(SPIFFS_FILENAME_MSG, FILE_WRITE);
//    if (!request->_tempFile) {
//      Serial.printf("Open file %s error!\n", SPIFFS_FILENAME_MSG);
//    }
  }

  if (len) {
//    ESP_LOGI(LOG_WEB, "(%d) %s", index, (const char*)data);
//    if (request->_tempFile) {
//      request->_tempFile.write(data, len);
//    }
  }
  
//  if (index + len >= total) {
//    if (request->_tempFile) {
//      request->_tempFile.close();
//    }
//    Serial.printf("BodyEnd: %u\n", total);
//  }
}
