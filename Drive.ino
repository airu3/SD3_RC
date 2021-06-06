// モーター動作開始
void Drive()
{
	bool called = 0;
	bool coil_state;

	SET_LED(1);

	while (1)
	{
		if (port(PB1))
			coil_state = 0;
		else
			coil_state = 1;

		SET_GPIO_OUT(ARM_SOLENOID, coil_state);
		SET_RC_STICK_DUTY(STICK1, STICK2);

#ifdef DEBUG_MODE
		Serial.print("coil "), Serial.print(coil_state);
		Serial.print("  S1:"), Serial.print(port(STICK1));
		Serial.print(", S2:"), Serial.print(port(STICK2));
		Serial.print(", M1:"), Serial.print(GET_M1_DUTY());
		Serial.print(", M2:"), Serial.print(GET_M2_DUTY());
		Serial.println();
#endif
	}
}
