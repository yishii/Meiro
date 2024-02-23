#ifndef __MEIRO_H__
#define __MRIRO_H__

#ifndef MEIRO_HW_REV
#define MEIRO_HW_REV 1
#endif

#include <Adafruit_NeoPixel.h>

#define FLOOR_B1 0
#define FLOOR_B2 1
#define FLOOR_B3 2
#define FLOOR_B4 3

class Meiro {
public:
  Meiro();
  void setLedColor(int led_no, int r, int g , int b);
  bool isBallTouch(int floor_no);

private:
  Adafruit_NeoPixel* pixels;
};


#endif /* __MEIRO_H__ */
