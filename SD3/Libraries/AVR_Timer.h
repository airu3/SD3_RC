// Created by Tomoki Sato
//=======================================================================
// pwm
//=======================================================================
#pragma once
#if defined(__AVR_ATmega328P__) // Arduino nano

#define DEBUG_TIMER1

#include <avr/io.h>
#define AVR_16MHz 16000000

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
		  OCR1A(PB1)
		  OCR1B(PB2)
	 タイマ/PWMの設定レジスタ:
		  TCCR1A
		  TCCR1B

タイマー2:
	 カウンター:
		  8bit
	 ピン出力値レジスタ(ピン名):
		  OCR2A(PB3)
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

//
void TIMER1_init()
{
	TCCR1A = 0;
	TCCR1B = 0;
}

//
void TIMER1_setup()
{
	TCCR1A = 0b10100010; // 10:コンペアマッチAでLOW,10:高速PWM動作
	// 制御レジスタB
	TCCR1B = 0b00011001; // 11:高速PWM動作, 001:分周なし
}

//
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

//
void OCR1B_setCompareMode(uint8_t type)
{
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

//
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

//
uint8_t TIMER1_getMode()
{
	return T1.mode;
}

//
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
		T1.clock = AVR_16MHz / 0;
	}
}

//
uint32_t TIMER1_getClock()
{
	return T1.clock;
}

//
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

//  pwm周波数を指定(周波数)[15000~60000][hz]
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

//
void AVR_SET_PWM9DUTY(uint8_t val)
{
	OCR1A = map(val, 0, 255, 0, T1.countTop);
#if defined(DEBUG_TIMER1)
	Serial.print(T1.countTop), Serial.print(" ");
	Serial.print(OCR1A), Serial.print(" ");
#endif
}

//
void AVR_SET_PWM10DUTY(uint8_t val)
{
	OCR1B = map(val, 0, 255, 0, T1.countTop);
#if defined(DEBUG_TIMER1)
	Serial.print(T1.countTop), Serial.print(" ");
	Serial.print(OCR1B), Serial.print(" ");
#endif
}

#endif