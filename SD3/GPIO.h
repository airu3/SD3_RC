// Created by Tomoki Sato
//=======================================================================
// デジタル入出力
//=======================================================================
#pragma once

//  pwm周波数を指定(周波数)[15000~60000][hz]
/**
 * @brief pwm周波数を指定(周波数)[15000~60000][hz]
 * @param Hz
 * @return void
 */
void SET_PWM_FREQ(uint16_t Hz)
{
#if IS_ARDUINO_NANO && !defined(OUTPUT_FULL_DIGITAL)
	if (Hz > 245)
	{
		uint32_t actualHz = AVR_SET_PWM3_PWM11FREQ(Hz);
		// 返ってきた "実際の" 周波数で 他のPWMも設定
		AVR_SET_PWM9_PWM10FREQ(actualHz);
	}

#elif IS_RP_PICO
	if (Hz > 2045)
		analogWriteFreq(Hz);

#endif
}

//[0|1] 入力状態を取得する(ピン番号)
/**
 * @brief [0|1] 入力状態を取得する(ピン番号)
 * @param uint8_t pin
 * @return bool
 * 		LOW : 0
 * 		HIGH : 1
 */
bool GET_GPIO_IN(uint8_t pin)
{
#if IS_ARDUINO_NANO
	return fastestDigitalRead(pin); // 高速デジタル入力

#elif IS_RP_PICO
	return gpio_get(pin); // pico 高速デジタル入力

#else
	return digitalRead(pin);

#endif
}

// LOWかHIGHの電圧を出力(ピン番号,LOW|HIGH)[0|1]
/**
 * @brief LOWかHIGHの電圧を出力(ピン番号,LOW|HIGH)[0|1]
 * @param uint8_t pin
 * @param bool val
 * @return void
 */
void SET_GPIO_OUT(uint8_t pin, bool val)
{
#if IS_ARDUINO_NANO
	fastestDigitalWrite(pin, val); // 高速デジタル出力

#elif IS_RP_PICO
	gpio_put(pin, val); // pico 高速デジタル出力
#else
	digitalWrite(pin, val);

#endif
}

//[1000~2000] パルス幅をマイクロ秒で取得(ピン番号)
/**
 * @brief [1000~2000] パルス幅をマイクロ秒で取得(ピン番号)
 * @param uint8_t pin
 * @return int
 */
int GET_GPIO_PULSE(uint8_t pin)
{
#if IS_ARDUINO_NANO
	// int pulse = pulseRead(pin); // pulseReadは安定しない場合があるため保留
	int pulse = pulseIn(pin, HIGH, 27500);

#elif IS_RP_PICO
	int pulse = pulseIn(pin, HIGH, 30000);
	// int pulse = pulseRead(pin); // pulseReadは安定しない場合があるため保留

#else
	int pulse = pulseIn(pin, HIGH, 30000);

#endif
	return (pulse > 0) ? pulse : 1500; // タイムアウト時は1500を返す
	return pulse;
}

// 指定したピンからアナログ値を出力(ピン番号,アナログ値)[pin,0~255]
/**
 * @brief 指定したピンからアナログ値を出力(ピン番号,アナログ値)[pin,0~255]
 * @param uint8_t pin
 * @param uint8_t val
 * @return void
 */
void SET_GPIO_PWM(uint8_t pin, uint8_t val)
{
// 完全デジタル入出力方式の場合
#if defined(OUTPUT_FULL_DIGITAL)
	SET_GPIO_OUT(pin, (val > 0) ? 1 : 0);

#elif IS_ARDUINO_NANO
	analogWrite(pin, val);

	// switch (pin)
	// {
	// case 3:
	// 	AVR_SET_PWM3DUTY(val);
	// 	break;
	// case 9:
	// 	AVR_SET_PWM9DUTY(val);
	// 	break;
	// case 10:
	// 	AVR_SET_PWM10DUTY(val);
	// 	break;
	// case 11:
	// 	AVR_SET_PWM11DUTY(val);
	// 	break;

	// default:
	// 	analogWrite(pin, val);
	// 	break;
	// }

#elif IS_RP_PICO
	analogWrite(pin, val);

#else
	analogWrite(pin, val);

#endif
	// Serial.print(val), Serial.print(" ");
}
