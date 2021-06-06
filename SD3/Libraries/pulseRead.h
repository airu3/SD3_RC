// Created by Tomoki Sato
//=======================================================================
// パルス入力高速化
/*
	課題
		フィルタリングの精度がいまいち
		スティック不感帯20%以下で不安定になる
		プログラムの処理時間でパルス計測に影響がある
*/
//=======================================================================
#pragma once

// パルスの変化を記録する
typedef struct
{
	uint32_t start;
	uint16_t width = 1500; // パルス幅(0~2000)[us]
	uint16_t width_L;		  // 最後のパルス幅(0~2000)[us]
	bool state;				  // パルスの状態[0|1]
	bool state_L;			  // 最後のパルスの状態[0|1]
} PULSE;

//[変換後のパルス] ローパスフィルタパルスの変化をなめらかにする(変換前のパルス)
PULSE LPF(PULSE pul)
{
	// kは係数(数値の変化を吸収する割合)[0.01~0.99]
	// 0.01~0.5 前回のパルス値を優先
	// 0.5~0.99 今回のパルス値を優先
	static float k;
	int diff = abs((long)(pul.width - pul.width_L));

	if (diff > 400)
		k = 0.16;
	else if (diff > 200)
		k = 0.32;
	else if (diff > 100)
		k = 0.16;
	else
		k = 0.02;

	pul.width = ((1.0 - k) * (float)pul.width_L) + (k * (float)pul.width);
	pul.width_L = pul.width;

	return pul;
}

//[1000~2000] パルスの幅を取得する(ピン番号)
int pulseRead(uint8_t i)
{
	unsigned int timeout = 30000;
	static PULSE pulse[30] = {0};

	pulse[i].state = digitalRead(i);

	if (pulse[i].state_L == 0 && pulse[i].state == 1)
	{
		pulse[i].start = micros();
	}
	else if (micros() - pulse[i].start > timeout)
	{
		pulse[i].width = 1500;
	}
	else if (pulse[i].state_L == 1 && pulse[i].state == 0)
	{
		pulse[i].width = micros() - pulse[i].start;
		pulse[i] = LPF(pulse[i]);
	}
	pulse[i].state_L = pulse[i].state;

	return pulse[i].width;
}