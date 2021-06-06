// Created by Tomoki Sato
//=======================================================================
// ピン設定
//=======================================================================
#pragma once

// ピン互換
//----------------------------------------------------------------------
//	Arduino nano <=> ラズパイピコ
/*
	A0 <=> 14
	A1 <=> 15
	A2 <=> 16
	A3 <=> 17
	A4 <=> 18
	A5 <=> 19
	A6 <=> 20
	A7 <=> 21
*/

// ピンアサイン
//----------------------------------------------------------------------
#define L1 0 // 左前方センサー
#define R1 1

#define L2 2 // 左斜めセンサー
#define R2 3
#define FW 4
// 5 モタドラ

// 6 モタドラ
// 7 モタドラ(緑のヒートシンク基板のみ)
// 8 モタドラ(緑のヒートシンク基板のみ)
#define L3 7 // 左横センサー
#define R3 8
// 9 モタドラ

// 10 モタドラ
#define lineL1 11 // 左床センサー
#define lineR1 12
// 13 アーム型 ソレノイド(Arduinoのみ)
#define ST_MODULE 13 // 自立型 スタート・ストップモジュール

#define FREE1 14	// 未定義1
#define FREE2 15	// 未定義2
#define STICK1 16 // ラジコン型 左スティック
#define STICK2 17 // ラジコン型 右スティック

// A5 ブザー(Arduinoのみ)
#define PB1 18 // コントローラー ボタン1
#define PB2 19
#define PB3 20
#define PB4 21

// 22 アーム型 ソレノイド
// 割当不可 23
// 割当不可 24
// 割当不可 25
#define FREE3 26 // 未定義3
#define FREE4 27 // 未定義4
// 28 ブザー

// モータードライバ
//----------------------------------------------------------------------
// OUT_DIR_4
// モーター1つでduty指定と回転方向[0,1|1,0]を各2ピンで指定する
/*
	例 BTS7960
	BTS7960_M1_LPWM 5	m1duty正転
	BTS7960_M1_RPWM 6	m1duty逆転
	BTS7960_M2_LPWM 9	m2duty正転
	BTS7960_M2_RPWM 10 m2duty逆転
*/

// OUT2_DIR2
//  モーター1つでduty指定が各1ピン、回転方向[0|1]を各1ピンで指定する
/*
	例 寺井さん基板
	TERAI_M1H 5	 m1のduty
	TERAI_M1P 6	 m1の回転方向
	TERAI_M2H 9	 m2のduty
	TERAI_M2P 10  m2の回転方向
*/

//	 OUT2_DIR4
// モーター1つでduty指定が各1ピン、回転方向[0,1|1,0]を各2ピンで指定する
/*
	例 OB購入 緑のヒートシンク基板
	OB_PWM_PA 5 m1duty
	OB_PWM_PB 6 m2duty
	OB_DIR_A1 7 m1正転
	OB_DIR_A2 8 m1逆転
	OB_DIR_B1 9 m2正転
	OB_DIR_B2 10 m2逆転
*/
// ピン
//----------------------------------------------------------------------
int state[30] = {0};

// 状態を取得(ピン番号)
/**
 * @brief ポートの状態を取得
 * @param i ポート番号
 * @return ポートの状態
 */
int update_port(uint8_t i)
{
	switch (i)
	{
	// コントローラー
	case STICK1:
	case STICK2:
		if (GET_ROBOT_TYPE() == RC)
			state[i] = GET_GPIO_PULSE(i);
		else
			state[i] = !GET_GPIO_IN(i);
		break;

	case PB1:
		if (GET_CONTROLLER_TYPE() == KO_MC8)
			state[i] = (GET_GPIO_PULSE(i) > 1750) ? 1 : 0;
		else if (GET_CONTROLLER_TYPE() == FS_NRC)
			state[i] = GET_GPIO_IN(i);
		else
			state[i] = !GET_GPIO_IN(i);
		break;

	case PB2:
	case PB3:
	case PB4:
		if (GET_CONTROLLER_TYPE() == FS_NRC)
			state[i] = GET_GPIO_IN(i);
		else
			state[i] = !GET_GPIO_IN(i);
		break;

	case ST_MODULE:
		state[i] = GET_GPIO_IN(i);
		break;

	// その他デジタルポート
	default:
		state[i] = !GET_GPIO_IN(i);
	}
	return state[i];
}

// ポートごとの入出力状態を取得(ピン番号)
/**
 * @brief ポートの入出力状態を取得
 * @param i ポート番号
 * @return ポートの入出力状態
 */
int port(uint8_t i)
{
	if (CANNOT_GET(i))
		return 0;
	else if (MULTI_PORT(i))
		return state[i];
	else
		return update_port(i);
}

// [ピン数] 指定範囲のポートが反応している数を取得(最初のピン,最後のピン)[0~,1~]
/**
 * @brief 指定範囲のポートが反応している数を取得
 * @param first 最初のピン
 * @param last 最後のピン
 * @return 反応している数
 */
uint8_t any_port(uint8_t first, uint8_t last)
{
	uint8_t count = 0;
	for (uint8_t i = first; i <= last; i++)
	{
		if (port(i))
			count++;
	}
	return count;
}

// 指定範囲のポート状態を一括で出力する(最初のピン,最後のピン)[0~,1~]
/**
 * @brief 指定範囲のポート状態を一括で出力する
 * @param first 最初のピン
 * @param last 最後のピン
 */
void print_port(uint8_t first, uint8_t last)
{
	for (uint8_t i = first; i <= last; i++)
	{
		Serial.print(port(i));
		if (i != last) // 最後の表示以外はカンマ区切り
			Serial.print(", ");
	}
	Serial.println();
}

// ラズパイピコ限定 マルチスレッド処理
#if defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED)
// マルチスレッド処理用の関数
void loop1()
{
	for (uint8_t i = 0; i <= GPIO_MAX; i++)
	{
		// 指定した並列処理用ポートだけを更新し続ける
		if (MULTI_PORT(i))
			update_port(i);
	}
}
#endif
