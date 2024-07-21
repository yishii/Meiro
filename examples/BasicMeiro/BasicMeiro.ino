/*
 * 基本の明路メインプログラムサンプル / Basic Meiro example
 */
#define MEIRO_HW_REV 5    // 明路ライブラリをRev.5用として使用する / Use the library for Meiro Rev.5
#include "Meiro.h"

Meiro meiro;

// setup : 電源をONにした直後に一度だけ実行する処理
void setup()
{
  meiro.begin(); // 明路ライブラリの初期化 / Initialize Meiro library
}

// loop : 何度も繰り返し実行する処理
void loop()
{
  // B1Fのピンにボールが触れたときの処理 / Process when a ball is touched to B1F pin
  if (meiro.isBallTouch(FLOOR_B1))
  {
    meiro.setLedColor(FLOOR_B1, 0, 0, 255);
    meiro.setLedOff(FLOOR_B2);
    meiro.setLedOff(FLOOR_B3);
    meiro.setLedOff(FLOOR_B4);
  }
 
  // B2Fのピンにボールが触れたときの処理 / Process when a ball is touched to B2F pin
  if (meiro.isBallTouch(FLOOR_B2))
  {
    meiro.setLedOff(FLOOR_B1);
    meiro.setLedColor(FLOOR_B2, 0, 255, 255);
    meiro.setLedOff(FLOOR_B3);
    meiro.setLedOff(FLOOR_B4);
  }

  // B3Fのピンにボールが触れたときの処理 / Process when a ball is touched to B3F pin
  if (meiro.isBallTouch(FLOOR_B3))
  {
    meiro.setLedOff(FLOOR_B1);
    meiro.setLedOff(FLOOR_B2);
    meiro.setLedColor(FLOOR_B3, 255, 0, 255);
    meiro.setLedOff(FLOOR_B4);
  }

  // B4Fのピンにボールが触れたときの処理 / Process when a ball is touched to B4F pin
  if (meiro.isBallTouch(FLOOR_B4))
  {
    meiro.setLedOff(FLOOR_B1);
    meiro.setLedOff(FLOOR_B2);
    meiro.setLedOff(FLOOR_B3);
    meiro.setLedColor(FLOOR_B4, 255, 255, 0);
  }

  // B5F(ゴール)のピンにボールが触れたときの処理 / Process when a ball is touched to B5F(Goal) pin
  if (meiro.isBallTouch(FLOOR_B5))
  {
    meiro.setLedColor(FLOOR_B1, 255, 255, 255);
    meiro.setLedColor(FLOOR_B2, 255, 255, 255);
    meiro.setLedColor(FLOOR_B3, 255, 255, 255);
    meiro.setLedColor(FLOOR_B4, 255, 255, 255);

    delay(500); // 500ミリ秒待つ / Wait for 500 milliseconds

    meiro.setLedOff(FLOOR_B1);
    meiro.setLedOff(FLOOR_B2);
    meiro.setLedOff(FLOOR_B3);
    meiro.setLedOff(FLOOR_B4);
  }
}
