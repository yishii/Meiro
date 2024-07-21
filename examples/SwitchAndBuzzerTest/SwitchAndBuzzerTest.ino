/*
 * 押しボタンスイッチとブザー動作のサンプル / Push switch and buzzer example
 * Meiro 6でのみ動作します / This example only works with Meiro 6
 */
#define MEIRO_HW_REV 6  // 明路ライブラリをRev.6用として使用する / Use the library for Meiro Rev.6
#include "Meiro.h"

Meiro meiro;

#define FREQ_C 523      // ドの音の周波数[Hz]
#define FREQ_G 784      // ソの音の周波数[Hz]

// setup : 電源をONにした直後に一度だけ実行する処理
void setup() {
  meiro.begin();  // 明路ライブラリの初期化 / Initialize Meiro library
}

// loop : 何度も繰り返し実行する処理
void loop() {
  if (meiro.isSw1Push()) { // SW1が押されていたら

    meiro.setLedColor(FLOOR_B2, 0, 255, 0); // LEDを緑に点灯させる

    meiro.tone(FREQ_C);     // ドの音を出力
    delay(100);             // 0.1秒待つ
    meiro.noTone();         // 音を止める

  } else if(meiro.isSw2Push()) { // SW2が押されていたら

    meiro.setLedColor(FLOOR_B2, 0, 0, 255); // LEDを青く点灯させる

    meiro.tone(FREQ_G);     // ソの音を出力
    delay(100);             // 0.1秒待つ
    meiro.noTone();         // 音を止める

  }
}
