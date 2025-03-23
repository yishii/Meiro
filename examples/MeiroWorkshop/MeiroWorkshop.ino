/*
 * 明路ワークショップ向けプログラム
 */
#include "Meiro.h"

#define FREQ_C 1046      // ドの音の周波数[Hz]
#define FREQ_G 1568      // ソの音の周波数[Hz]

Meiro meiro;

// setup : 電源をONにした直後に一度だけ実行する処理
void setup()
{
  meiro.begin(); // 明路ライブラリの初期化
}

// loop : 何度も繰り返し実行する処理
void loop()
{
  // ★B1Fのピンにボールが触れたときの処理
  if (meiro.isBallTouch(FLOOR_B1))
  {
    meiro.setLedColor(FLOOR_B1, 255, 255, 255); // B1は、R=255、G=255、B=255の白を設定
    meiro.setLedColor(FLOOR_B2, 0, 0, 0); // B2は、R=0、G=0、B=0の黒を設定
    meiro.setLedColor(FLOOR_B3, 0, 0, 0); // B3は、R=0、G=0、B=0の黒を設定
    meiro.setLedColor(FLOOR_B4, 0, 0, 0); // B4は、R=0、G=0、B=0の黒を設定

    meiro.tone(FREQ_C);     // ドの音を出力
    delay(100);             // 0.1秒待つ
    meiro.noTone();         // 音を止める
  }
 
  // ★B2Fのピンにボールが触れたときの処理
  if (meiro.isBallTouch(FLOOR_B2))
  {
    meiro.setLedColor(FLOOR_B1, 0, 0, 0); // B1は、R=0、G=0、B=0の黒を設定
    meiro.setLedColor(FLOOR_B2, 255, 255, 255); // B2は、R=255、G=255、B=255の白を設定
    meiro.setLedColor(FLOOR_B3, 0, 0, 0); // B3は、R=0、G=0、B=0の黒を設定
    meiro.setLedColor(FLOOR_B4, 0, 0, 0); // B4は、R=0、G=0、B=0の黒を設定

    meiro.tone(FREQ_C);     // ドの音を出力
    delay(100);             // 0.1秒待つ
    meiro.noTone();         // 音を止める
  }

}
