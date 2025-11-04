// Created by Tomoki Sato
//=======================================================================
// pwm (Arduino nano専用)
// ===============================
// ⚠ INTERNAL ONLY / 触るな危険 ⚠
// 高校生向け教材では **このファイルはいじらない**こと。
// 変更したい場合はメンテ担当の指導者に相談してください。
// ===============================
//=======================================================================
#pragma once
#if defined(__AVR_ATmega328P__) // Arduino nano

#if defined(DEBUG_TIMER1)
#define __DEBUG_TIMER1__ 1
else
#define __DEBUG_TIMER1__ 0
#endif

#if defined(DEBUG_TIMER2)
#define __DEBUG_TIMER2__ 1
		else
#define __DEBUG_TIMER2__ 0
#endif

#include <avr/io.h>
#define AVR_16MHz 16000000UL
		//=======================================================================

		// https://usicolog.nomaki.jp/engineering/avr/avrPWM.html
		// https://geek.tacoskingdom.com/blog/52
		/*
		タイマー0:
			 カウンター:
					8bit
			 ピン出力値レジスタ(ピン名):
					OCR0A(PD6)
					OCR0B(PD5)
			 タイマ/PWMの設定レジスタ:
					TCCR0A
					TCCR0B

		タイマー1:
			 カウンター:
					16bit
			 ピン出力値レジスタ(ピン名):
					OCR1A(BTN1)
					OCR1B(BTN2)
			 タイマ/PWMの設定レジスタ:
					TCCR1A
					TCCR1B

		タイマー2:
			 カウンター:
					8bit
			 ピン出力値レジスタ(ピン名):
					OCR2A(BTN3)
					OCR2B(PD3)
			 タイマ/PWMの設定レジスタ:
					TCCR2A
					TCCR2B
		*/

		//
		typedef struct
{
	uint8_t mode;
	uint32_t clock;
	uint16_t countTop;
} AVR_TIMER;
AVR_TIMER T1;

//
enum TIMER1_MODE
{
	Fast_PWM_ICR1 = 14, // Fast PWM Mode, TOP = ICR1
};
/*
「標準モード」:
	 いわゆる普通のタイマー。
	 8bitと16bitの２種類があり、カウンタとして動作させたい時に使用

「CTCモード」:
	 コンペアマッチ(指定値とタイマーカウントが一致)を使うモード。
	 この指定値はOCR●AかOCR●Bで設定する。
	 タイマーカウントがその指定値を超えたら矩形波形が出力電圧を反転し、
	 同時にタイマは0に戻る

「高速PWMモード」:
	 PWM波形を出力する際に使用。
	 ただしタイマー波形はノコギリ波ベースとなる

「位相基準PWMモード」:
	 こちらもPWM波形を出力する際に使用。
	 ただしタイマー波形は三角波ベースとなる
*/
/*
WGM13-WGM12-WGM11-WGM10:
	 0000 ... 標準・TOP(0xFFFF)
	 0001 ... 8bit位相標準PWM・TOP(0x00FF)
	 0010 ... 9bit位相標準PWM・TOP(0x01FF)
	 0011 ... 10bit位相標準PWM・TOP(0x03FF)
	 1000 ... 位相/周波数標準PWM・TOP(ICR1)
	 1001 ... 位相/周波数標準PWM・TOP(OCR1A)
	 1010 ... 位相標準PWM・TOP(ICR1)
	 1011 ... 位相標準PWM・TOP(OCR1A)
	 0100 ... CTC・TOP(ICR1)
	 1100 ... CTC・TOP(OCR1A)
	 1101 ... 無し
	 0101 ... 8bit高速PWM・TOP(0x00FF)
	 0110 ... 9bit高速PWM・TOP(0x01FF)
	 0111 ... 10bit高速PWM・TOP(0x03FF)
	 1110 ... 高速PWM・TOP(ICR1)
	 1111 ... 高速PWM・TOP(OCR1A)
*/
enum TIMER1_CLOCK_PRESCALER
{
	div1 = 1,
	div8,
	div64,
	div256,
	div1024,
};
/*
CS12-CS11-CS10: クロックの分周特性
	 000 ... タイマー停止
	 001 ... 分周無し(=1)
	 010 ... 1/8
	 011 ... 1/64
	 100 ... 1/256
	 101 ... 1/1024
	 110 ... 外部クロックT1立ち下がりエッジ
	 111 ... 外部クロックT1立ち上がりエッジ
*/

//
enum TIMER1_COMPARE_MODE
{
	TOGGLE = 1,
	NON_INVERT,
	INVERT
};
/*
COM1A1-COM1A0: OC1Aピンの出力モード
	 00 ... 出力なし
	 01 ... コンペアマッチでトグル
	 10 ... コンペアマッチでLOW(ノン・インバートモード)
	 11 ... コンペアマッチでHIGH(インバートモード)

COM1B1-COM1B0: OC1Bピンの出力モード
	 00 ... 出力なし
	 01 ... コンペアマッチでトグル
	 10 ... コンペアマッチでLOW(ノン・インバートモード)
	 11 ... コンペアマッチでHIGH(インバートモード)
*/

// 初期化
/**
 * @brief 初期化
 */
void TIMER1_init()
{
	TCCR1A = 0;
	TCCR1B = 0;
}

/**
 * @brief タイマー1設定
 */
void TIMER1_setup()
{
	TCCR1A = 0b10100010; // 10:コンペアマッチAでLOW,10:高速PWM動作
	// 制御レジスタB
	TCCR1B = 0b00011001; // 11:高速PWM動作, 001:分周なし
}

/**
 * @brief コンペアマッチAの出力モード設定
 * @param type 出力モード
 * @details
 * COM1A1-COM1A0: OC1Aピンの出力モード
	 00 ... 出力なし
	 01 ... コンペアマッチでトグル
	 10 ... コンペアマッチでLOW(ノン・インバートモード)
	 11 ... コンペアマッチでHIGH(インバートモード)
 */
void OCR1A_setCompareMode(uint8_t type)
{
	switch (type)
	{
	case TOGGLE:
		TCCR1A |= (0 << COM1A1) | (1 << COM1A0);
		break;

	case NON_INVERT:
		TCCR1A |= (1 << COM1A1) | (0 << COM1A0);
		break;

	case INVERT:
		TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
		break;

	default:
		TCCR1A |= (0 << COM1A1) | (0 << COM1A0);
	}
}

/**
 * @brief コンペアマッチBの出力モード設定
 * @param type 出力モード
 * @details
 * COM1B1-COM1B0: OC1Bピンの出力モード
	 00 ... 出力なし
	 01 ... コンペアマッチでトグル
	 10 ... コンペアマッチでLOW(ノン・インバートモード)
	 11 ... コンペアマッチでHIGH(インバートモード)
 */
void OCR1B_setCompareMode(uint8_t type)
{
	// COM1A1, COM1A0 を一度クリア
	TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
	switch (type)
	{
	case TOGGLE:
		TCCR1A |= (0 << COM1B1) | (1 << COM1B0);
		break;

	case NON_INVERT:
		TCCR1A |= (1 << COM1B1) | (0 << COM1B0);
		break;

	case INVERT:
		TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
		break;

	default:
		TCCR1A |= (0 << COM1B1) | (0 << COM1B0);
	}
}

/**
 * @brief タイマー1モード設定
 * @param mode モード
 * @details
 * タイマー1の動作モードを設定します。
 */
void TIMER1_setMode(uint8_t mode)
{
	T1.mode = mode;
	switch (mode)
	{
	case Fast_PWM_ICR1:
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		TCCR1A |= (1 << WGM11) | (0 << WGM10);
		break;
	}
}

/**
 * @brief タイマー1の動作モードを取得
 * @return タイマー1の動作モード
 */
uint8_t TIMER1_getMode()
{
	return T1.mode;
}

/**
 * @brief タイマー1のクロックを設定
 * @param type 分周設定
 */
void TIMER1_setClock(uint8_t type)
{
	switch (type)
	{
	case div1:
		TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
		T1.clock = AVR_16MHz / 1;
		break;

	case div8:
		TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
		T1.clock = AVR_16MHz / 8;
		break;

	case div64:
		TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
		T1.clock = AVR_16MHz / 64;
		break;

	case div256:
		TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
		T1.clock = AVR_16MHz / 256;
		break;

	case div1024:
		TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
		T1.clock = AVR_16MHz / 1024;
		break;

	default:
		TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10);
		T1.clock = 0; // 停止
	}
}

/**
 * @brief タイマー1のクロックを取得
 * @return タイマー1のクロック
 */
uint32_t TIMER1_getClock()
{
	return T1.clock;
}

/**
 * @brief カウンター設定(周波数)[Hz]
 * @param Hz 周波数
 */
void TIMER1_setCounter(uint32_t Hz)
{
	uint32_t clock = TIMER1_getClock();

	if (Hz > 0 && clock > 0)
		T1.countTop = (uint16_t)((clock / Hz) - 1);

	switch (TIMER1_getMode())
	{
	case Fast_PWM_ICR1:
		// 最大値
		ICR1 = T1.countTop;
		break;
	}
}

/**
 * @brief pwm周波数を指定(周波数)[15000~60000][hz]
 * @param Hz
 * @return void
 */
void AVR_SET_PWM9_PWM10FREQ(uint32_t Hz)
{
	if (Hz >= 250 && Hz <= 60000)
	{
		TIMER1_init();
		OCR1A_setCompareMode(NON_INVERT);
		OCR1B_setCompareMode(NON_INVERT);
		TIMER1_setMode(Fast_PWM_ICR1);
		TIMER1_setClock(div1);
		TIMER1_setCounter(Hz);
	}
#if defined(DEBUG_TIMER1)
	Serial.println(TCCR1A, BIN);
	Serial.println(TCCR1B, BIN);
	Serial.println(TIMER1_getClock());
	Serial.println(T1.countTop);
#endif
}

/**
 * @brief PWMデューティ設定(0~255)
 * @param val デューティ値(0~255)
 * @return void
 */
void AVR_SET_PWM9DUTY(uint8_t val)
{
	OCR1A = map(val, 0, 255, 0, T1.countTop);
#if defined(DEBUG_TIMER1)
	Serial.print(T1.countTop), Serial.print(" ");
	Serial.print(OCR1A), Serial.print(" ");
#endif
}

/**
 * @brief PWMデューティ設定(0~255)
 * @param val デューティ値(0~255)
 * @return void
 */
void AVR_SET_PWM10DUTY(uint8_t val)
{
	OCR1B = map(val, 0, 255, 0, T1.countTop);
#if defined(DEBUG_TIMER1)
	Serial.print(T1.countTop), Serial.print(" ");
	Serial.print(OCR1B), Serial.print(" ");
#endif
}

// 新規: Timer2 用の分周定義（PWM3/11 用）
// Timer2の使用可能な分周比を正しく定義
enum TIMER2_CLOCK_PRESCALER
{
	T2_div1 = 1,			// CS2[2:0] = 001
	T2_div8 = 8,			// CS2[2:0] = 010
	T2_div32 = 32,		// CS2[2:0] = 011
	T2_div64 = 64,		// CS2[2:0] = 100
	T2_div128 = 128,	// CS2[2:0] = 101
	T2_div256 = 256,	// CS2[2:0] = 110
	T2_div1024 = 1024 // CS2[2:0] = 111
};

/**
 * @brief PWM3 (OC2B) と PWM11 (OC2A) の周波数設定（Fast PWM, TOP = 0xFF）
 * @param targetHz 狙いのHz
 * @return actualHz 実際に設定されたHz
 */
uint32_t AVR_SET_PWM3_PWM11FREQ(uint32_t targetHz)
{
	if (targetHz == 0)
		return 0;

	const uint32_t clock = 16000000UL;
	const uint32_t prescalers[7] = {1, 8, 32, 64, 128, 256, 1024};
	const uint8_t prescalerBits[7] = {
			(1 << CS20),														// /1
			(1 << CS21),														// /8
			(1 << CS21) | (1 << CS20),							// /32
			(1 << CS22),														// /64
			(1 << CS22) | (1 << CS20),							// /128
			(1 << CS22) | (1 << CS21),							// /256
			(1 << CS22) | (1 << CS21) | (1 << CS20) // /1024
	};

	// 最も近い周波数となる分周比を選択
	int chosen = 0;
	uint32_t min_diff = UINT32_MAX;

	for (int i = 0; i < 7; i++)
	{
		uint32_t freq = clock / (prescalers[i] * 256);
		uint32_t diff = (freq > targetHz) ? (freq - targetHz) : (targetHz - freq);
		if (diff < min_diff)
		{
			min_diff = diff;
			chosen = i;
		}
	}

	// レジスタ初期化
	TCCR2A = 0;
	TCCR2B = 0;

	// D11(OC2A), D3(OC2B) 非反転出力
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);

	// Fast PWM mode, TOP = 0xFF (WGM2[2:0] = 011)
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	// WGM22 = 0 (すでに0)

	// 分周設定
	TCCR2B |= prescalerBits[chosen];

#if defined(DEBUG_TIMER2)
	Serial.print("TCCR2A: ");
	Serial.println(TCCR2A, BIN);
	Serial.print("TCCR2B: ");
	Serial.println(TCCR2B, BIN);
	Serial.print("prescaler: ");
	Serial.println(prescalers[chosen]);
#endif

	// 実際の周波数を計算して返す
	return clock / (prescalers[chosen] * 256);
}

/**
 * @brief PWM3(D3, OC2B)のデューティ設定(0~255)
 * @param val デューティ値(0~255)
 */
void AVR_SET_PWM3DUTY(uint8_t val)
{
	OCR2B = val; // 直接値を設定（TOP=0xFFなのでマップ不要）
#if defined(DEBUG_TIMER2)
	Serial.print("PWM3 duty: ");
	Serial.println(val);
#endif
}

/**
 * @brief PWM11(D11, OC2A)のデューティ設定(0~255)
 * @param val デューティ値(0~255)
 */
void AVR_SET_PWM11DUTY(uint8_t val)
{
	OCR2A = val; // 直接値を設定（TOP=0xFFなのでマップ不要）
#if defined(DEBUG_TIMER2)
	Serial.print("PWM11 duty: ");
	Serial.println(val);
#endif
}

#endif