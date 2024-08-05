/*
 * Meiro library
 *
 * Copyright (C) Yasuhiro ISHII
 */
#ifndef __MEIRO_H__
#define __MRIRO_H__

#include <Adafruit_NeoPixel.h>

#if !defined(MEIRO_HW_REV)
#error Please define MEIRO_HW_REV in your sketch!
#endif

#if MEIRO_HW_REV == 5
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
#elif MEIRO_HW_REV == 6
// Definition for pin number
#define _PIN_B1 1
#define _PIN_B2 2
#define _PIN_B3 3
#define _PIN_B4 4
#define _PIN_B5 5
#define _PIN_LED 8
#define _PIN_SW1 0
#define _PIN_SW2 6
#define _PIN_BUZZER 7

// Definition for serial LED connecting order
#define _LED_B1 3
#define _LED_B2 2
#define _LED_B3 1
#define _LED_B4 0

#define BUTTON_LONG_PRESS_THRESH 500
#else
#error Meiro Hardware Revision other than 5 and 6 is not supported so far.
#endif

#define FLOOR_B1 0
#define FLOOR_B2 1
#define FLOOR_B3 2
#define FLOOR_B4 3
#define FLOOR_B5 4

#define MAX_FLOORS 4

class Meiro {
public:
  Meiro()
  {
    memset(colors_specified, 0, sizeof(colors_specified));
  };

  void begin(void)
  {
    pinMode(_PIN_B1, INPUT_PULLUP);
    pinMode(_PIN_B2, INPUT_PULLUP);
    pinMode(_PIN_B3, INPUT_PULLUP);
    pinMode(_PIN_B4, INPUT_PULLUP);
    pinMode(_PIN_B5, INPUT_PULLUP);

#if MEIRO_HW_REV == 6
    pinMode(_PIN_SW1, INPUT_PULLUP);
    pinMode(_PIN_SW2, INPUT_PULLUP);
    digitalWrite(_PIN_BUZZER, LOW);
    pinMode(_PIN_BUZZER, OUTPUT);
#endif

    privilegedMode = false;

    pixels = new Adafruit_NeoPixel(4, _PIN_LED, NEO_GRB + NEO_KHZ800);
    pixels->begin();
    pixels->clear();

    // Set the all LEDs RGB(0, 0, 0)
    setLedOff(FLOOR_B1);
    setLedOff(FLOOR_B2);
    setLedOff(FLOOR_B3);
    setLedOff(FLOOR_B4);

#if MEIRO_HW_REV == 6
    xTaskCreate(Meiro::TaskBackgroundMainFunction, "Meiro Background Main Task", 2048, this, 1, NULL);
#endif

  };

  void backgroundMain(void)
  {
    static bool sw1_1d = false;
    static bool sw2_1d = false;
    static uint32_t sw1_pressed_time = 0;
    static uint32_t sw2_pressed_time = 0;
    bool communicationMode = false;
    uint32_t timeModeEntered = 0;

    while(1)
    {
      bool sw1 = isSw1Push();
      bool sw2 = isSw2Push();
      bool sw1_short_pressed = false;
      bool sw2_short_pressed = false;
      bool sw1_long_pressed = false;
      bool sw2_long_pressed = false;

      if (sw1 && !sw1_1d)
      {
        sw1_pressed_time = millis();
      }

      if (sw2 && !sw2_1d)
      {
        sw2_pressed_time = millis();
      }

      if (!sw1 && sw1_1d)
      {
        if ((millis() - sw1_pressed_time) > BUTTON_LONG_PRESS_THRESH)
        {
          sw1_long_pressed = true;
        } else {
          sw1_short_pressed = true;
        }
      }

      if (!sw2 && sw2_1d)
      {
        if ((millis() - sw2_pressed_time) > BUTTON_LONG_PRESS_THRESH)
        {
          sw2_long_pressed = true;
        } else {
          sw2_short_pressed = true;
        }
      }

      if(sw1_long_pressed){
        Serial.println("SW1 LONG");
      }
      if(sw2_long_pressed){
        Serial.println("SW2 LONG");
      }
      if(sw1_short_pressed){
        Serial.println("SW1 SHORT");
      }
      if(sw2_short_pressed){
        Serial.println("SW2 SHORT");
      }

      {
        if (!communicationMode){
          if(sw1_long_pressed){
            Serial.println("Enter");
            communicationMode = true;
            timeModeEntered = millis();
            privilegedMode = true;
            for (int i=0; i<MAX_FLOORS; i++){
              setLedColorInternal(i, 0, 0, 255);
            }
          }
        } else {
          if((sw1_long_pressed) || ((millis() - timeModeEntered) > (60 * 1000))){
            Serial.println("Leave");
            communicationMode = false;
            privilegedMode = false;
            restoreLedColors();
          }
        }
      }

      sw1_1d = sw1;
      sw2_1d = sw2;

      vTaskDelay(10 / portTICK_RATE_MS);
    }
  }

  void setLedColor(int floor_no, int r, int g , int b)
  {
    const int led_no_lut[] =
    {
      _LED_B1,
      _LED_B2,
      _LED_B3,
      _LED_B4
    };
    if((floor_no >= FLOOR_B1) && (floor_no <= FLOOR_B4)) {
      colors_specified[floor_no].r = r;
      colors_specified[floor_no].g = g;
      colors_specified[floor_no].b = b;
      if(!privilegedMode){
        pixels->setPixelColor(led_no_lut[floor_no], pixels->Color(r, g, b));
        pixels->show();
      }
    }
  };

  void restoreLedColors(void)
  {
    const int led_no_lut[] =
    {
      _LED_B1,
      _LED_B2,
      _LED_B3,
      _LED_B4
    };
    int floor;

    for(floor=0; floor<MAX_FLOORS; floor++)
    {
      pixels->setPixelColor(led_no_lut[floor],
        pixels->Color(colors_specified[floor].r, colors_specified[floor].g, colors_specified[floor].b));
    }
    pixels->show();
  };

  void setLedOff(int led_no)
  {
    this->setLedColor(led_no, 0, 0, 0);
  };

  bool isSw1Push(void)
  {
#if MEIRO_HW_REV == 6
    if(digitalRead(_PIN_SW1) == LOW)
    {
      return true;
    } else {
      return false;
    }
#else
    return false;
#endif
  };

  bool isSw2Push(void)
  {
#if MEIRO_HW_REV == 6
    if(digitalRead(_PIN_SW2) == LOW)
    {
      return true;
    } else {
      return false;
    }
#else
    return false;
#endif
  };

  void tone(uint32_t frequency, uint32_t duration = 0)
  {
#if MEIRO_HW_REV == 6
    ::tone(_PIN_BUZZER, frequency, duration);
#endif
  };

  void noTone()
  {
#if MEIRO_HW_REV == 6
    ::noTone(_PIN_BUZZER);
#endif
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
  };

private:
  Adafruit_NeoPixel* pixels;
#if MEIRO_HW_REV == 6
  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } colors_specified[MAX_FLOORS]; // スケッチで指定されたLED色情報
  portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;

  static void TaskBackgroundMainFunction(void* pvParameters) {
    Meiro* instance = static_cast<Meiro*>(pvParameters);
    instance->backgroundMain();
  }
  bool privilegedMode = false;

  void setLedColorInternal(int floor_no, int r, int g , int b)
  {
    const int led_no_lut[] =
    {
      _LED_B1,
      _LED_B2,
      _LED_B3,
      _LED_B4
    };
    if((floor_no >= FLOOR_B1) && (floor_no <= FLOOR_B4)) {
      pixels->setPixelColor(led_no_lut[floor_no], pixels->Color(r, g, b));
      pixels->show();
    }
  };
#endif
};

#endif /* __MEIRO_H__ */
