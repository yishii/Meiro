/*
 * Blue Meiro
 */
#define MEIRO_HW_REV 1    // Use the library for Meiro Rev.1 / 明路ライブラリをRev.1用として使用する
#include "Meiro.h"

Meiro meiro;

#define LED_FALLDOWN_TIME_MS  200
#define LED_FADEOUT_TIME_MS   20

const static int led_lut[] = {
  FLOOR_B1, FLOOR_B2, FLOOR_B3, FLOOR_B4
};

// setup : 電源をONにした直後に一度だけ実行する処理
void setup()
{
  // Do nothing / 特に何もしない
}

void lightFloor(int active_floor)
{
  static uint8_t colors[4][3];
  static int state;
  static int substate;
  static uint32_t last_state_changed_time;
  static uint32_t last_fadeout_time;
  int led;
  uint32_t current_time = millis();

  int tmp;
  
  if((last_state_changed_time + LED_FALLDOWN_TIME_MS) < current_time) {
    colors[state][0] = 0;
    colors[state][1] = 0;
    colors[state][2] = 255;
    state++;
    if (state == 4) {
      state = 0;
    }
    last_state_changed_time = current_time;
  }

  if((last_fadeout_time + LED_FADEOUT_TIME_MS) < current_time) {
    for(led=0; led<4; led++) {
      tmp = colors[led][0] - 10; colors[led][0] = (tmp >= 0) ? tmp : 0;
      tmp = colors[led][1] - 10; colors[led][1] = (tmp >= 0) ? tmp : 0;
      tmp = colors[led][2] - 10; colors[led][2] = (tmp >= 0) ? tmp : 0;
    }
    for(led=0; led<4; led++) {
      if (led != active_floor) {
        meiro.setLedColor(led_lut[led], colors[led][0], colors[led][1], colors[led][2]);
      } else {
        meiro.setLedColor(led_lut[led], 255, 255, 255);
      }
    }
    last_fadeout_time = current_time;
  }
}

void reachedToGoal(void)
{
  int led;
  int light;
  for(light=0; light<256; light++){
    for(led=0; led<4; led++) {
      meiro.setLedColor(led_lut[led], light, light, light);
    }
    delay(8);
  }
  for(light=0; light<256; light++){
    for(led=0; led<4; led++) {
      meiro.setLedColor(led_lut[led], 255-light, 255-light, 255-light);
    }
    delay(8);
  }
}

// loop : 何度も繰り返し実行する処理
void loop()
{
  static int active_floor = -1;
  // Process when a ball is touched to B1F pin / B1Fのピンにボールが触れたときの処理
  if (meiro.isBallTouch(FLOOR_B1))
  {
    active_floor = FLOOR_B1;
  }
 
  // Process when a ball is touched to B2F pin / B2Fのピンにボールが触れたときの処理
  else if (meiro.isBallTouch(FLOOR_B2))
  {
    active_floor = FLOOR_B2;
  }

  // Process when a ball is touched to B3F pin / B3Fのピンにボールが触れたときの処理
  else if (meiro.isBallTouch(FLOOR_B3))
  {
    active_floor = FLOOR_B3;
  }

  // Process when a ball is touched to B4F pin / B4Fのピンにボールが触れたときの処理
  else if (meiro.isBallTouch(FLOOR_B4))
  {
    active_floor = FLOOR_B4;
  }

  // Process when a ball is touched to B5F pin / B5Fのピンにボールが触れたときの処理
  else if (meiro.isBallTouch(FLOOR_B5))
  {
    reachedToGoal();
    active_floor = -1;
  }

  lightFloor(active_floor);
}
