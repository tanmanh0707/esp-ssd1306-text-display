#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <FS.h>

typedef enum {
  FS_OPEN_WRITE = (0),
  FS_OPEN_READ,
  FS_OPEN_READWRITE,
  FS_OPEN_MAX
} FsOpenType_e;

bool FS_Init();
File FS_Open(const char *path, FsOpenType_e type);
bool FS_Write(File &fs, const char *data, size_t len);
bool FS_Close(File &fs);
size_t FS_FileSize(File &fs);
size_t FS_Read(File &fs, char *data, size_t len);

bool SCREEN_Init();
void SCREEN_Display();
void SCREEN_DrawMultiLineText(String &text, uint32_t start_line = 0);
