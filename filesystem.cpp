#include "common.h"
#include <SPI.h>
#include <FS.h>
#if defined(ESP32)
#include <SD.h>
#include <SPIFFS.h>
#endif

bool FS_Init()
{
#if defined(ESP32)
  SPIFFS.begin(true);
#elif defined(ESP8266)
  SPIFFS.begin();
#endif
  return true;
}

File FS_Open(const char *path, FsOpenType_e type)
{
#if defined(ESP32)
  File file = SPIFFS.open(path, (type == FS_OPEN_WRITE)? FILE_WRITE : FILE_READ);
#elif defined(ESP8266)
  File file = SPIFFS.open(path, (type == FS_OPEN_WRITE)? "w" : "r");
#endif
  return file;
}

bool FS_Write(File &fs, const char *data, size_t len)
{
#if defined(ESP32)
  fs.write((const uint8_t *)data, len);
#elif defined(ESP8266)
  fs.write(data, len);
#endif
  return true;
}

bool FS_Close(File &fs)
{
  fs.close();
  return true;
}

size_t FS_Read(File &fs, char *data, size_t len)
{
  return fs.readBytes(data, len);
}

size_t FS_FileSize(File &fs)
{
  unsigned long filesize = 0;

  if (!fs || fs.isDirectory()) {
      //ESP_LOGE(LOG_COMMON, "Failed to open file for reading: %s", path);
  } else {
    filesize = fs.size();
  }

  return filesize;
}
