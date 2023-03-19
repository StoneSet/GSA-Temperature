#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define OLED_RESET 6
Adafruit_SH1106 display(OLED_RESET);

// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4

tmElements_t tm;

const unsigned char epd_bitmap_engine [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x3e, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0x60, 0x00, 0x03, 0x7e, 
  0x00, 0x60, 0x00, 0x03, 0x60, 0x00, 0x3c, 0x00, 0x07, 0x60, 0x00, 0x06, 0x00, 0x07, 0x60, 0x00, 
  0x06, 0xf0, 0x07, 0x60, 0x00, 0x06, 0xb0, 0x07, 0xe0, 0x00, 0x07, 0x90, 0x03, 0xe0, 0x00, 0x07, 
  0x98, 0x03, 0xe0, 0x00, 0x00, 0x18, 0x03, 0x60, 0x00, 0x00, 0x18, 0x03, 0x60, 0x00, 0x00, 0x18, 
  0x03, 0x60, 0x00, 0x07, 0x98, 0x03, 0x7e, 0x00, 0x07, 0x90, 0x00, 0x3f, 0x00, 0x06, 0xf0, 0x00, 
  0x03, 0x80, 0x06, 0xf0, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00
};

const unsigned char splash [] PROGMEM = {
// 'splash', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x7e, 0x00, 0xfc, 0x00, 0xc0, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xe7, 0x81, 0xcf, 0x00, 0xe0, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xc0, 0x00, 0x03, 0x81, 0xc3, 0x03, 0x00, 0xe0, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0x00, 0x03, 0x01, 0xc3, 0x03, 0x01, 0xe0, 0x00, 0x00, 
0x00, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xc0, 0x00, 0x07, 0x01, 0x83, 0x03, 0x01, 0xf0, 0x00, 0x00, 
0x00, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x07, 0x00, 0x03, 0x00, 0x01, 0x30, 0x00, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x06, 0x00, 0x03, 0xc0, 0x03, 0x30, 0x00, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x06, 0x00, 0x01, 0xf0, 0x03, 0x38, 0x00, 0x00, 
0x00, 0xff, 0xfc, 0x00, 0x0f, 0xff, 0xc0, 0x00, 0x06, 0x00, 0x00, 0xfc, 0x02, 0x18, 0x00, 0x00, 
0x00, 0xff, 0xf8, 0x0c, 0x07, 0xff, 0xc0, 0x00, 0x06, 0x07, 0xc0, 0x3e, 0x06, 0x18, 0x00, 0x00, 
0x00, 0xff, 0xf0, 0x3e, 0x07, 0xff, 0xc0, 0x00, 0x06, 0x01, 0xc0, 0x0f, 0x07, 0xfc, 0x00, 0x00, 
0x00, 0xff, 0xf0, 0x7f, 0x83, 0xff, 0xc0, 0x00, 0x07, 0x01, 0xc0, 0x07, 0x07, 0xfc, 0x00, 0x00, 
0x00, 0xff, 0xe0, 0xff, 0xc1, 0xff, 0xc0, 0x00, 0x07, 0x01, 0xc6, 0x03, 0x0c, 0x0e, 0x00, 0x00, 
0x00, 0xff, 0xc1, 0xf7, 0xe0, 0xff, 0xc0, 0x00, 0x07, 0x01, 0xc6, 0x03, 0x0c, 0x0e, 0x00, 0x00, 
0x00, 0xff, 0x83, 0xf3, 0xf0, 0x7f, 0xc0, 0x00, 0x03, 0x81, 0xc6, 0x03, 0x08, 0x06, 0x00, 0x00, 
0x00, 0xff, 0x0f, 0xe1, 0xf8, 0x7f, 0xc0, 0x00, 0x01, 0xc3, 0xc7, 0x06, 0x18, 0x07, 0x00, 0x00, 
0x00, 0xff, 0x1f, 0xc0, 0xfc, 0x3f, 0xc0, 0x00, 0x00, 0xff, 0x01, 0xfc, 0x7f, 0x1f, 0x80, 0x00, 
0x00, 0xfe, 0x3f, 0x80, 0xff, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xfc, 0x7f, 0x00, 0x7f, 0x8f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xf8, 0xff, 0x00, 0x3f, 0xcf, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xf9, 0xfe, 0x00, 0x1f, 0xe7, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xf7, 0xfc, 0x00, 0x0f, 0xf3, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xef, 0xf8, 0x0c, 0x0f, 0xf9, 0xc0, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xf8, 0x1e, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0x9c, 0x00, 0x0e, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xf0, 0x3f, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x31, 0xc8, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xe0, 0x7f, 0x81, 0xff, 0xc0, 0x00, 0x00, 0x60, 0xd8, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xc1, 0xff, 0xc1, 0xff, 0xc0, 0x00, 0x00, 0x61, 0x98, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xc3, 0xff, 0xf0, 0xff, 0xc0, 0x00, 0x00, 0xc0, 0x18, 0xc3, 0x06, 0xe0, 0x00, 0x00, 
0x00, 0xff, 0x87, 0xff, 0xf8, 0x7f, 0xc0, 0x00, 0x00, 0xc0, 0x19, 0xc3, 0x05, 0x30, 0x00, 0x00, 
0x00, 0xff, 0x0f, 0xff, 0xfc, 0x3f, 0xc0, 0x00, 0x00, 0xc0, 0x18, 0xc3, 0x06, 0x30, 0x00, 0x00, 
0x00, 0xfe, 0x1f, 0xff, 0xfe, 0x1f, 0xc0, 0x00, 0x00, 0xc0, 0x10, 0xc3, 0x0e, 0x30, 0x00, 0x00, 
0x00, 0xfc, 0x7f, 0xff, 0xff, 0x1f, 0xc0, 0x00, 0x01, 0xc0, 0x30, 0xc2, 0x0c, 0x30, 0x00, 0x00, 
0x00, 0xfc, 0xff, 0xff, 0xff, 0x8f, 0xc0, 0x00, 0x01, 0xc0, 0x30, 0xc6, 0x0c, 0x30, 0x00, 0x00, 
0x00, 0xf9, 0xff, 0xff, 0xff, 0xe7, 0xc0, 0x00, 0x00, 0xc0, 0x30, 0x86, 0x0c, 0x30, 0x00, 0x00, 
0x00, 0xf3, 0xff, 0xff, 0xff, 0xf3, 0xc0, 0x00, 0x00, 0xc1, 0x31, 0x8e, 0x0c, 0x60, 0x00, 0x00, 
0x00, 0xe7, 0xff, 0xff, 0xff, 0xfb, 0xc0, 0x00, 0x00, 0xe3, 0xb9, 0x8f, 0x08, 0xc0, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xc0, 0x00, 0x00, 0x7c, 0x38, 0xf7, 0x8f, 0x80, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'screen_cache_calandre', 128x64px
const unsigned char epd_bitmap_screen_cache_calandre [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x3c, 0x30, 0x3d, 0xef, 0xfe, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x64, 0x30, 0x64, 0xc6, 0x62, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xc0, 0x58, 0xc0, 0xc6, 0x60, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xc0, 0x58, 0xc0, 0xc6, 0x64, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xc0, 0x58, 0xc0, 0xfe, 0x7c, 0x00, 0x00, 
  0x03, 0xff, 0xfc, 0xff, 0xef, 0xff, 0xf8, 0x00, 0x00, 0xc0, 0xfc, 0xc0, 0xc6, 0x64, 0x00, 0x00, 
  0x03, 0xff, 0xe0, 0x7f, 0xc0, 0xff, 0xf8, 0x00, 0x00, 0xc0, 0x8c, 0xc0, 0xc6, 0x60, 0x00, 0x00, 
  0x07, 0xff, 0x80, 0x7f, 0xc0, 0x3f, 0xf8, 0x00, 0x00, 0x64, 0x8c, 0x64, 0xc6, 0x62, 0x00, 0x00, 
  0x07, 0xfe, 0x00, 0x7f, 0xc0, 0x0f, 0xfc, 0x00, 0x00, 0x3d, 0xde, 0x3d, 0xef, 0xfe, 0x00, 0x00, 
  0x07, 0xf8, 0x00, 0xff, 0xc0, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x07, 0xf0, 0x00, 0xff, 0xc0, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x07, 0xe0, 0x01, 0xff, 0xf0, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x07, 0xc0, 0x07, 0xff, 0xfc, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x07, 0xc0, 0x1f, 0xff, 0xff, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0f, 0xc0, 0x7f, 0xff, 0xff, 0xc0, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0f, 0xc0, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0f, 0xc3, 0xff, 0xc0, 0xff, 0xf8, 0x7e, 0x07, 0x8c, 0x3c, 0x0c, 0x38, 0xff, 0x87, 0xe7, 0xf0, 
  0x0f, 0xc7, 0xff, 0x80, 0x3f, 0xfc, 0xfe, 0x0c, 0x8c, 0x18, 0x0c, 0x1c, 0x4c, 0xc3, 0x33, 0x10, 
  0x0f, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xfe, 0x18, 0x16, 0x18, 0x16, 0x1c, 0x4c, 0x63, 0x33, 0x00, 
  0x0f, 0xff, 0xfc, 0x00, 0x0f, 0xff, 0xfe, 0x18, 0x16, 0x18, 0x16, 0x16, 0x4c, 0x63, 0x33, 0x20, 
  0x1f, 0xff, 0xf8, 0x00, 0x07, 0xff, 0xfe, 0x18, 0x16, 0x18, 0x16, 0x13, 0x4c, 0x63, 0xe3, 0xe0, 
  0x1f, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0x18, 0x3f, 0x18, 0x3f, 0x13, 0x4c, 0x63, 0x63, 0x20, 
  0x1f, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x18, 0x23, 0x18, 0x23, 0x11, 0xcc, 0x63, 0x33, 0x00, 
  0x1f, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0x0c, 0xa3, 0x19, 0x23, 0x11, 0xcc, 0xc3, 0x33, 0x10, 
  0x1f, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x07, 0xf7, 0xbf, 0x77, 0xb8, 0xdf, 0x87, 0xbf, 0xf0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Addresses of 3 DS18B20s
//uint8_t sensor1[8] = { 0x28, 0xE4, 0x7A, 0x57, 0x04, 0xD6, 0x3C, 0x14 }; //engine
//uint8_t sensor2[8] = { 0x28, 0x2B, 0x3F, 0x57, 0x04, 0xA0, 0x3C, 0x88 }; //OUT
//uint8_t sensor3[8] = { 0x28, 0x1C, 0x27, 0x57, 0x04, 0xE1, 0x3D, 0xA3 }; //IN

void setup()   {

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.drawBitmap(0, 0, splash, 128, 64, WHITE);
  display.display();
  delay(5000);
  
// pour le cache calandre  

  sensors.requestTemperatures();
  int temp_out = sensors.getTempCByIndex(0);

  if (temp_out <= 6) {
    display.clearDisplay();
    display.drawBitmap(0, 0, epd_bitmap_screen_cache_calandre, 128, 64, WHITE);
    display.display();
    delay(4000);
  }
  display.clearDisplay();
}

void loop() {

  RTC.read(tm);
  
  // temperature
  sensors.requestTemperatures();

  //DateTime now = rtc.now();
  //char heure[10];
  //sprintf(heure, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  int temp_engine = sensors.getTempCByIndex(2);
  int temp_out = sensors.getTempCByIndex(0);
  int temp_in = sensors.getTempCByIndex(1);

  //draw text
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_engine, 40, 23, WHITE);
  display.setCursor(50,5);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.print(temp_engine);
  display.println("C");
  display.setCursor(0,30);
  //temperature in et out (deuxieme ligne)
  display.setTextSize(2);
  display.print("IN");
  display.println(temp_in);
  display.setCursor(60,30);
  display.print("OUT");
  display.print(temp_out);
  //heure
  display.setCursor(18,50);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(tm.Hour);
  display.print(":");
  display.print(tm.Minute);
  display.print(":");
  display.print(tm.Second);
  //display.println(heure);
  display.display();
  delay(1000);
}

// https://tutoduino.fr/blog-rtc/
// https://pecquery.wixsite.com/arduino-passion/copie-de-le-detecteur-a-ultrasons-h-2
// https://javl.github.io/image2cpp/
// https://www.hackster.io/keval-doshi/arduino-oled-temperature-display-with-real-time-clock-5ff644
// https://electropeak.com/learn/interfacing-sh1106-1-3-inch-i2c-oled-128x64-display-with-arduino/
