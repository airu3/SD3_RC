// Created by Tomoki Sato
//=======================================================================
// 基板側 固定ピンアサイン
//=======================================================================
#pragma once

// #define OUTPUT_FULL_DIGITAL

// モータードライバ
//----------------------------------------------------------------------
// OUT_DIR_4
// BTS7960など
// モーター1つでduty指定と回転方向[0,1|1,0]を各2ピンで指定する
#define MD1_M1_OUT1 5  // m1duty正転
#define MD1_M1_OUT2 6  // m1duty逆転
#define MD1_M2_OUT1 9  // m2duty正転
#define MD1_M2_OUT2 10 // m2duty逆転

// OUT2_DIR2
// 寺井さん基板など
//  モーター1つでduty指定が各1ピン、回転方向[0|1]を各1ピンで指定する
#define MD2_M1_OUT 5	 // m1duty
#define MD2_M1_DIR 6	 // m1回転方向
#define MD2_M2_OUT 9	 // m2duty
#define MD2_M2_DIR 10 // m2回転方向

// OUT2_DIR4
// 緑のヒートシンク基板など
// モーター1つでduty指定が各1ピン、回転方向[0,1|1,0]を各2ピンで指定する
#define MD3_M1_DIR1 5 // m1正転
#define MD3_M1_DIR2 6 // m1逆転
#define MD3_M2_DIR1 7 // m2正転
#define MD3_M2_DIR2 8 // m2逆転
#define MD3_M1_OUT 9	 // m1duty
#define MD3_M2_OUT 10 // m2duty

// DIPスイッチ
//----------------------------------------------------------------------
#define DIP1 18 // 最下位ビット
#define DIP2 19
#define DIP3 20
#define DIP4 21 // 最上位ビット

// アーム用ソレノイド
//----------------------------------------------------------------------
#if defined(__AVR_ATmega328P__) // Arduino nano
#define ARM_SOLENOID 13
#elif defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED) // ラズパイピコ
#define ARM_SOLENOID 22
#endif

// ブザー
//----------------------------------------------------------------------
#if defined(__AVR_ATmega328P__) // Arduino nano
#define BUZZER A5
#elif defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED) // ラズパイピコ
#define BUZZER 28
#endif
