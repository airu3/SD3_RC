#include "pulseRead.h"

// プロポのピン設定
#define STICK_THROTTLE 4  // D4: スロットル入力
#define STICK_STEERING 5  // D5: ステアリング入力

void setup() {
  // シリアル初期化
  Serial.begin(115200);
  
  // プロポ入力ピンを設定
  pinMode(STICK_THROTTLE, INPUT);
  pinMode(STICK_STEERING, INPUT);
  
  // パルス読み取り初期化
  initPulseRead();
}

void loop() {
  // プロポの値を読み取り
  int throttle = pulseRead(STICK_THROTTLE);  // 1000-2000μs
  int steering = pulseRead(STICK_STEERING);  // 1000-2000μs
  
  // デバッグ出力
  Serial.print("Throttle: ");
  Serial.print(throttle);
  Serial.print("us, Steering: ");
  Serial.print(steering);
  Serial.println("us");
  
  // 実用的な変換例：
  // 1000-2000μs → -100%～+100%
  int throttle_percent = map(throttle, 1000, 2000, -100, 100);
  int steering_percent = map(steering, 1000, 2000, -100, 100);
  
  delay(20);  // 適度な更新間隔
}
