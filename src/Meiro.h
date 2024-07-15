/*
 * Meiro library
 *
 * Copyright (C) Yasuhiro ISHII
 */
#ifndef __MEIRO_H__
#define __MRIRO_H__

#include <Adafruit_NeoPixel.h>

#if !defined(MEIRO_HW_REV)
#error スケッチ内に「MEIRO_HW_REV」を適切に定義してください。Please define MEIRO_HW_REV in your sketch!
#endif

#if MEIRO_HW_REV == 1
// Definition for pin number
#define _PIN_B1 7
#define _PIN_B2 9
#define _PIN_B3 8
#define _PIN_B4 1
#define _PIN_B5 0
#define _PIN_LED 3

// Definition for serial LED connecting order
#define _LED_B1 3
#define _LED_B2 2
#define _LED_B3 1
#define _LED_B4 0
#elif MEIRO_HW_REV == 2
// Definition for pin number
#define _PIN_B1 1
#define _PIN_B2 2
#define _PIN_B3 3
#define _PIN_B4 4
#define _PIN_B5 5
#define _PIN_LED 8

// Definition for serial LED connecting order
#define _LED_B1 3
#define _LED_B2 2
#define _LED_B3 1
#define _LED_B4 0
#else
#error Meiro Hardware Revision other than 1 and 2 is not supported so far.
#endif

#define FLOOR_B1 0
#define FLOOR_B2 1
#define FLOOR_B3 2
#define FLOOR_B4 3
#define FLOOR_B5 4

class Meiro {
public:
  Meiro()
  {
  };

  void begin(void)
  {
    pinMode(_PIN_B1, INPUT_PULLUP);
    pinMode(_PIN_B2, INPUT_PULLUP);
    pinMode(_PIN_B3, INPUT_PULLUP);
    pinMode(_PIN_B4, INPUT_PULLUP);
    pinMode(_PIN_B5, INPUT_PULLUP);

    pixels = new Adafruit_NeoPixel(4, _PIN_LED, NEO_GRB + NEO_KHZ800);
    pixels->begin();
    pixels->clear();

    // Set the all LEDs RGB(0, 0, 0)
    pixels->setPixelColor(_LED_B1, pixels->Color(0, 0, 0));
    pixels->setPixelColor(_LED_B2, pixels->Color(0, 0, 0));
    pixels->setPixelColor(_LED_B3, pixels->Color(0, 0, 0));
    pixels->setPixelColor(_LED_B4, pixels->Color(0, 0, 0));
    pixels->show();
  }

  void setLedColor(int led_no, int r, int g , int b)
  {
    const int led_no_lut[] =
    {
      _LED_B1,
      _LED_B2,
      _LED_B3,
      _LED_B4
    };
    if((led_no >= FLOOR_B1) && (led_no <= FLOOR_B4)) {
      pixels->setPixelColor(led_no_lut[led_no], pixels->Color(r, g, b));
      pixels->show();
    }
  };

  void setLedOff(int led_no)
  {
    this->setLedColor(led_no, 0, 0, 0);
  };

  bool isBallTouch(int floor_no)
  {
    bool ret = false;
    switch (floor_no) {
      case FLOOR_B1:
        if (digitalRead(_PIN_B1) == LOW) {
          ret = true;
        }
        break;
      case FLOOR_B2:
        if (digitalRead(_PIN_B2) == LOW) {
          ret = true;
        }
        break;
      case FLOOR_B3:
        if (digitalRead(_PIN_B3) == LOW) {
          ret = true;
        }
        break;
      case FLOOR_B4:
        if (digitalRead(_PIN_B4) == LOW) {
          ret = true;
        }
        break;
      case FLOOR_B5:
        if (digitalRead(_PIN_B5) == LOW) {
          ret = true;
        }
        break;
      default:
        break;
    }
    return ret;
  }

private:
  Adafruit_NeoPixel* pixels;
};

#endif /* __MEIRO_H__ */
