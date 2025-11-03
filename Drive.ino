// モーター動作開始
void Drive()
{
	bool coil_state; // ソレノイド状態
	SET_LED(1);			 // 動作開始LED点灯

	while (1)
	{
		// 押している間だけソレノイド展開
		// coil_state = (port(PB1)) ? 0 : 1;
		// SET_GPIO_OUT(ARM_SOLENOID, coil_state);

		// 2ch RC入力を取得
		SET_RC_WHEEL_DUTY(STICK1, STICK2);

#ifdef DEBUG_MODE
		// Serial.print("coil "), Serial.print(coil_state);
		Serial.print("  S1:"), Serial.print(port(STICK1));
		Serial.print(", S2:"), Serial.print(port(STICK2));
		Serial.print(", M1:"), Serial.print(GET_M1_DUTY());
		Serial.print(", M2:"), Serial.print(GET_M2_DUTY());
		Serial.println();
#endif
	}
}
