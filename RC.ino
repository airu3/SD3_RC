// ラジコン制御

// コントローラーのスティック値をdutyに変換(スティック値1,スティック値2)[上下軸,上下軸]
void SET_RC_STICK_DUTY(uint8_t pin1, uint8_t pin2)
{
	static int pulse1 = 0, pulse2 = 0;
	static int m1duty = 0, m2duty = 0;

	pulse1 = port(pin1);
	pulse2 = port(pin2);

	m1duty = STICK_TO_DUTY(pulse1);
	m2duty = STICK_TO_DUTY(pulse2);

	if (m1duty > 0)
		m1duty = 1000;
	else if (m1duty < 0)
		m1duty = -1000;
	else
	{
		m1duty = 0;
		Timer_reset();
		Motion_reset();
	}

	if (m2duty > 0)
		m2duty = 1000;
	else if (m2duty < 0)
		m2duty = -1000;
	else
	{
		m2duty = 0;
		Timer_reset();
		Motion_reset();
	}
	SET_DUTY(m1duty, m2duty);
}

// コントローラーのスティック値をdutyに変換(スティック値1,スティック値2)[左右軸,上下軸]
void SET_RC_WHEEL_DUTY(uint8_t pin1, uint8_t pin2)
{
	static int throttle = 0, steer = 0;
	static int m1duty = 0, m2duty = 0;

	steer = port(pin1);
	throttle = port(pin2);

	m1duty = STICK_TO_DUTY(steer);
	m2duty = STICK_TO_DUTY(throttle);

	// 旋回
	if (throttle > 0) // 前進
	{
		// 片軸の出力を下げる
		if (steer < 0) // 左旋回(マイナス値)
			m1duty = 0, m2duty = 1000;
		else if (0 < steer) // 右旋回(プラス値)
			m1duty = 1000, m2duty = 0;
		else
		{
			m1duty = 1000, m2duty = 1000;
			Timer_reset();
			Motion_reset();
		}
	}
	else if (throttle < 0) // 後退
	{
		// 片軸の出力を上げる
		if (steer < 0) // 左バック
			m1duty = -1000, m2duty = 0;
		else if (0 < steer) // 右バック
			m1duty = 0, m2duty = -1000;
		else
		{
			m1duty = -1000, m2duty = -1000;
			Timer_reset();
			Motion_reset();
		}
	}
	else // 停止状態のとき
	{
		// その場で旋回
		if (steer < 0) // 左バック
			m1duty = -1000, m2duty = 0;
		else if (0 < steer) // 右バック
			m1duty = 0, m2duty = -1000;
		else
		{
			m1duty = 0, m2duty = 0;
			Timer_reset();
			Motion_reset();
		}
	}
	SET_DUTY(m1duty, m2duty);
}
