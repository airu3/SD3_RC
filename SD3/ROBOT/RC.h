// Created by Tomoki Sato
//=======================================================================
// ラジコン
//=======================================================================
#pragma once

#define LL 0 // lower limit 上限
#define UL 1 // upper limit 下限

// スティックの計測値
typedef struct
{
	int min = 1000;
	int neutral = 1500;
	int max = 2000;
	int dead_zone[2] = {1250, 1750}; // スティック不感帯((最大値-中間値)*5~95)[%]
} STICK;
STICK S;

// スティック可動領域の不感帯を指定する(5~95)[%]
/**
 * @brief スティック可動領域の不感帯を指定する(5~95)[%]
 * @param percent 不感帯の割合[%]
 * @return なし
 *
 */
void SET_STICK_DEAD_ZONE(float percent)
{
	if (percent >= 5 && percent <= 95)
	{
		S.dead_zone[LL] = S.neutral - (float)(S.neutral - S.min) * (percent / 100.0);
		S.dead_zone[UL] = S.neutral + (float)(S.max - S.neutral) * (percent / 100.0);
	}
}

//[0|1] スティックの計測値が不感帯かどうかを判定する
/**
 * @brief スティックの計測値が不感帯かどうかを判定する
 * @param pulse スティックの計測値
 * @return 不感帯かどうか[0|1]
 *
 */
bool DEAD_ZONE(uint16_t pulse)
{
	return (S.dead_zone[LL] <= pulse) && (pulse <= S.dead_zone[UL]);
}

//[-1000~1000] スティック計測値をdutyに変換(パルス値)[1000~2000]
/**
 * @brief スティック計測値をdutyに変換(パルス値)[1000~2000]
 * @param pulse スティックの計測値
 * @return duty値[-1000~1000]
 *
 */
int STICK_TO_DUTY(uint16_t pulse)
{
	static int duty = 0;

	if (DEAD_ZONE(pulse))
		duty = 0;
	else // スティック入力値[最小値~最大値]をduty値[-1000~1000]に変換
		duty = map(pulse, S.min, S.max, -1000, 1000);

	return duty;
}