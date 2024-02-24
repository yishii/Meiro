/*
 * Basic Meiro example(not completed, for hands-on seminar) / 基本の明路メインプログラムサンプル(未完成版)
 */
#define MEIRO_HW_REV 1    // Use the library for Meiro Rev.1 / 明路ライブラリをRev.1用として使用する
#include "Meiro.h"

Meiro meiro;

// setup : 電源をONにした直後に一度だけ実行する処理
void setup()
{
  // Do nothing / 特に何もしない
}

// loop : 何度も繰り返し実行する処理
void loop()
{
  // Process when a ball is touched to B1F pin / B1Fのピンにボールが触れたときの処理
  if (meiro.isBallTouch(FLOOR_B1))
  {
    meiro.setLedColor(FLOOR_B1, 0, 0, 255);
    meiro.setLedOff(FLOOR_B2);
    meiro.setLedOff(FLOOR_B3);
    meiro.setLedOff(FLOOR_B4);
  }
 
  // Process when a ball is touched to B2F pin / B2Fのピンにボールが触れたときの処理
  if (meiro.isBallTouch(FLOOR_B2))
  {
    meiro.setLedOff(FLOOR_B1);
    meiro.setLedColor(FLOOR_B2, 0, 255, 255);
    meiro.setLedOff(FLOOR_B3);
    meiro.setLedOff(FLOOR_B4);
  }

  // Let's write a code for other floors in the same way! / 同様に他のフロアのひかりかたを作ってみよう!

}
