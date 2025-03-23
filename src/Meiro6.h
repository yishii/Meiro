/*
 * Meiro 6 library
 *
 * Copyright (C) Yasuhiro ISHII
 */
#ifndef __MEIRO6_H__
#define __MRIRO6_H__

#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

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

#define SERVICE_UUID "711e7d1f-0022-4b3c-9eb1-4fd781201183"
#define CHARACTERISTIC_UUID "e3e68008-a279-4ecc-99f1-630f9ca57c38"

typedef enum
{
  COMMAND_NOP = 0,
  COMMAND_SET_COLOR = 1,
} CommandCode;

#define FLOOR_B1 0
#define FLOOR_B2 1
#define FLOOR_B3 2
#define FLOOR_B4 3
#define FLOOR_B5 4

#define MAX_FLOORS 4

class Meiro;
class BleCallbacks;

class BleCallbacks : public BLECharacteristicCallbacks
{
private:
  void onWrite(BLECharacteristic *pCharacteristic);

public:
  BleCallbacks(Meiro *m);
  bool isAccessed(void)
  {
    return accessed;
  }

private:
  bool accessed = false;
  Meiro *meiro = NULL;
};

class Meiro
{
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

    pinMode(_PIN_SW1, INPUT_PULLUP);
    pinMode(_PIN_SW2, INPUT_PULLUP);
    digitalWrite(_PIN_BUZZER, LOW);
    pinMode(_PIN_BUZZER, OUTPUT);

    privilegedMode = false;

    pixels = new Adafruit_NeoPixel(4, _PIN_LED, NEO_GRB + NEO_KHZ800);
    pixels->begin();
    pixels->clear();

    // Set the all LEDs RGB(0, 0, 0)
    setLedOff(FLOOR_B1);
    setLedOff(FLOOR_B2);
    setLedOff(FLOOR_B3);
    setLedOff(FLOOR_B4);

    xTaskCreate(Meiro::TaskBackgroundMainFunction, "Meiro Background Main Task", 8192, this, 1, NULL);
  };

  void backgroundMain(void);
  void startBle(void);
  void stopBle(void);

  void setLedColor(int floor_no, int r, int g, int b)
  {
    const int led_no_lut[] =
        {
            _LED_B1,
            _LED_B2,
            _LED_B3,
            _LED_B4};
    if ((floor_no >= FLOOR_B1) && (floor_no <= FLOOR_B4))
    {
      colors_specified[floor_no].r = r;
      colors_specified[floor_no].g = g;
      colors_specified[floor_no].b = b;
      if (!privilegedMode)
      {
        pixels->setPixelColor(led_no_lut[floor_no], pixels->Color(r, g, b));
        pixels->show();
      }
    }
  };

  void restoreLedColors(void)
  {
    const int led_no_lut[] = {
      _LED_B1,
      _LED_B2,
      _LED_B3,
      _LED_B4
    };
    int floor;

    for (floor = 0; floor < MAX_FLOORS; floor++)
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
    if (digitalRead(_PIN_SW1) == LOW)
    {
      return true;
    }
    else
    {
      return false;
    }
  };

  bool isSw2Push(void)
  {
    if (digitalRead(_PIN_SW2) == LOW)
    {
      return true;
    }
    else
    {
      return false;
    }
  };

  void tone(uint32_t frequency, uint32_t duration = 0)
  {
    ::tone(_PIN_BUZZER, frequency, duration);
  };

  void noTone()
  {
    ::noTone(_PIN_BUZZER);
  };

  bool isBallTouch(int floor_no)
  {
    bool ret = false;
    switch (floor_no)
    {
    case FLOOR_B1:
      if (digitalRead(_PIN_B1) == LOW)
      {
        ret = true;
      }
      break;
    case FLOOR_B2:
      if (digitalRead(_PIN_B2) == LOW)
      {
        ret = true;
      }
      break;
    case FLOOR_B3:
      if (digitalRead(_PIN_B3) == LOW)
      {
        ret = true;
      }
      break;
    case FLOOR_B4:
      if (digitalRead(_PIN_B4) == LOW)
      {
        ret = true;
      }
      break;
    case FLOOR_B5:
      if (digitalRead(_PIN_B5) == LOW)
      {
        ret = true;
      }
      break;
    default:
      break;
    }
    return ret;
  };

  void setLedColorInternal(int floor_no, int r, int g, int b)
  {
    const int led_no_lut[] =
        {
            _LED_B1,
            _LED_B2,
            _LED_B3,
            _LED_B4};
    if ((floor_no >= FLOOR_B1) && (floor_no <= FLOOR_B4))
    {
      pixels->setPixelColor(led_no_lut[floor_no], pixels->Color(r, g, b));
      pixels->show();
    }
  };

private:
  Adafruit_NeoPixel *pixels;
  struct
  {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } colors_specified[MAX_FLOORS]; // スケッチで指定されたLED色情報
  portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;

  static void TaskBackgroundMainFunction(void *pvParameters)
  {
    Meiro *instance = static_cast<Meiro *>(pvParameters);
    instance->backgroundMain();
  }
  bool privilegedMode = false;

  BleCallbacks *bleCallbacks = NULL;
  BLEService *pBleService = NULL;
};

#endif /* __MEIRO6_H__ */
