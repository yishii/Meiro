/*
 * Meiro library
 *
 * Copyright (C) Yasuhiro ISHII
 */
#include "Meiro.h"

#if MEIRO_HW_REV == 1
// Definition for pin number
#define PIN_B1 7
#define PIN_B2 9
#define PIN_B3 8
#define PIN_B4 1
#define PIN_B5 0
#define PIN_LED 3

// Definition for serial LED connecting order
#define LED_B1 3
#define LED_B2 2
#define LED_B3 1
#define LED_B4 0
#else
#erro Meiro Hardware Revision other than 1 is not supported so far.
#endif

Meiro::Meiro() {
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);
  pinMode(PIN_B3, INPUT_PULLUP);
  pinMode(PIN_B4, INPUT_PULLUP);
  pinMode(PIN_B5, INPUT_PULLUP);

  pixels = new Adafruit_NeoPixel(4, PIN_LED, NEO_GRB + NEO_KHZ800);
  pixels->begin();
  pixels->clear();

  // Set the all LEDs RGB(0, 0, 0)
  pixels->setPixelColor(LED_B1, pixels->Color(0, 0, 0));
  pixels->setPixelColor(LED_B2, pixels->Color(0, 0, 0));
  pixels->setPixelColor(LED_B3, pixels->Color(0, 0, 0));
  pixels->setPixelColor(LED_B4, pixels->Color(0, 0, 0));
  pixels->show();
}

void Meiro::setLedColor(int led_no, int r, int g, int b) {
  const int led_no_lut[] =
  {
    LED_B1,
    LED_B2,
    LED_B3,
    LED_B4
  };
  pixels->setPixelColor(led_no_lut[led_no], pixels->Color(r, g, b));
  pixels->show();
}

bool Meiro::isBallTouch(int floor_no) {
  bool ret = false;
  switch (floor_no) {
    case FLOOR_B1:
      if (digitalRead(PIN_B1) == LOW) {
        ret = true;
      }
      break;
    case FLOOR_B2:
      if (digitalRead(PIN_B2) == LOW) {
        ret = true;
      }
      break;
    case FLOOR_B3:
      if (digitalRead(PIN_B3) == LOW) {
        ret = true;
      }
      break;
    case FLOOR_B4:
      if (digitalRead(PIN_B4) == LOW) {
        ret = true;
      }
      break;
    default:
      break;
  }
  return ret;
}
