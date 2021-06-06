// Created by Tomoki Sato
//=======================================================================
// 基本動作
//=======================================================================

// 動作状態の管理
typedef struct
{
	int8_t state; // 現在の動作[FORWARD|BACK|...]
	int8_t step;  // 複合動作のステップ数[0|1|2|...]
} MOTION;
MOTION Motion;

// 動作保持用の変数(関数名+方向指定の大文字)
enum MOTION_STATE
{
	// 数字はユーザーenum定義と被らないように指定している
	FORWARD = -128,
	BACK,
	STOP,
	TURN_L,
	TURN_R,
	SPIN_TURN_L,
	SPIN_TURN_R,
};

// 動作の方向指定
// 例: Motion = spin_turn(L, 100);
// 例: Motion = spin_turn(R, 100);
// 例: Motion = turn(L, 100);
// 例: Motion = turn(R, 100);
enum MOVE_DIRECTION
{
	L = 1,
	R = 2,
};

// 動作状態をリセットする
/**
 * @brief 動作状態をリセットする
 * @return void
 * @details
 * この関数は、動作状態をリセットするために使用する
*/
void Motion_reset()
{
	Motion = (MOTION){0};
}

// 動作を次に進める
/**
 * @brief 動作を次に進める
 * @return void
 * @details
 * この関数は、複合動作のステップ数を進めるために使用する
*/
void Motion_next()
{
	Motion.state = 0;
	Motion.step++;
}

// 時間指定のできるSET_DUTY(m1出力値,m2出力値,指定時間)[-1000~1000,-1000~1000,ms]
/**
 * @brief 時間指定のできるSET_DUTY(m1出力値,m2出力値,指定時間)[-1000~1000,-1000~1000,ms]
 * @param m1duty m1出力値[-1000~1000]
 * @param m2duty m2出力値[-1000~1000]
 * @param time 指定時間[ms]
 * @return MOTION
 * @details
 * この関数は、指定時間だけm1,m2の出力値を指定した値に設定します。
 * 指定時間が経過すると、m1,m2の出力値を0に設定し、次のステップに進みます。
 * 
 * 例: Motion = timer_set_duty(1000, 1000, 1000); // m1,m2の出力値を1000に設定し、1秒後に0に設定する
 * 
 * 
*/
MOTION timer_set_duty(int m1duty, int m2duty, uint16_t time)
{
	SET_DUTY(m1duty, m2duty);

	if (Timer.start == 0)
	{
		Timer.start = millis();
		Timer.end = time;
	}
	else if (millis() - Timer.start >= Timer.end)
	{
		Timer_reset();
		Motion.step++;
	}

	Motion.state = 0;
	return Motion;
}

// 前進(指定時間)[ms]
/**
 * @brief 前進(指定時間)[ms]
 * @param time 指定時間[ms]
 * @return MOTION FORWARD
 * @details
 * この関数は、指定時間だけ前進します。
 * 指定時間が経過すると、次のステップに進みます。
 *
 */
MOTION forward(uint16_t time)
{
	SET_DUTY(1000, 1000);

	if (Timer.start == 0)
	{
		Timer.start = millis();
		Timer.end = time;
	}
	else if (millis() - Timer.start >= Timer.end)
	{
		Timer_reset();
		Motion_next();
		return Motion;
	}

	if (port(L2) || port(R2))
	{
		Timer_reset();
		Motion_next();
	}
	else
		Motion.state = FORWARD;

	return Motion;
}

// 後退(指定時間)[ms]
/**
 * @brief 後退(指定時間)[ms]
 * @param time 指定時間[ms]
 * @return MOTION BACK
 * @details
 * この関数は、指定時間だけ後退します。
 * 指定時間が経過すると、次のステップに進みます。
 *
 */
MOTION back(uint16_t time)
{
	SET_DUTY(-1000, -1000);

	if (Timer.start == 0)
	{
		Timer.start = millis();
		Timer.end = time;
	}
	else if (millis() - Timer.start >= Timer.end)
	{
		Timer_reset();
		Motion_next();
		return Motion;
	}

	Motion.state = BACK;
	return Motion;
}

// 停止(指定時間)[ms]
/**
 * @brief 停止(指定時間)[ms]
 * @param time 指定時間[ms]
 * @return MOTION STOP
 * @details
 * この関数は、指定時間だけ停止します。
 * 指定時間が経過すると、次のステップに進みます。
 *
 */
MOTION stop(uint16_t time)
{
	SET_DUTY(0, 0);

	if (Timer.start == 0)
	{
		Timer.start = millis();
		Timer.end = time;
	}
	else if (millis() - Timer.start >= Timer.end)
	{
		Timer_reset();
		Motion_next();
		return Motion;
	}

	if (port(L2) || port(R2))
	{
		Timer_reset();
		Motion_next();
	}
	else
		Motion.state = STOP;

	return Motion;
}

// 片輪で旋回(方向,指定時間)[L|R|-L|-R,ms]
/**
 * @brief 片輪で旋回(方向,指定時間)[L|R|-L|-R,ms]
 * @param L_R L|R|-L|-R
 * @param time 指定時間[ms]
 * @return MOTION TURN
 * @details
 * この関数は、指定時間だけ片輪で旋回します。
 * 指定時間が経過すると、次のステップに進みます。
 *
 */
MOTION turn(int8_t L_R, uint16_t time)
{
	if (L_R == L)
		SET_DUTY(0, 1000);
	else if (L_R == R)
		SET_DUTY(1000, 0);
	else if (L_R == -L)
		SET_DUTY(-1000, 0);
	else if (L_R == -R)
		SET_DUTY(0, -1000);

	if (Timer.start == 0)
	{
		Timer.start = millis();
		Timer.end = time;
	}
	else if (millis() - Timer.start >= Timer.end)
	{
		Timer_reset();
		Motion_next();
		return Motion;
	}

	if (port(L2) || port(R2))
	{
		Timer_reset();
		Motion_next();
	}
	else
		Motion.state = (L_R == L || L_R == -L) ? TURN_L : TURN_R;

	return Motion;
}

// その場で旋回(方向,指定時間)[L|R,ms]
/**
 * @brief その場で旋回(方向,指定時間)[L|R,ms]
 * @param L_R L|R
 * @param time 指定時間[ms]
 * @return MOTION SPIN_TURN
 * @details
 * この関数は、指定時間だけその場で旋回します。
 * 指定時間が経過すると、次のステップに進みます。
 *
 */

MOTION spin_turn(int8_t L_R, uint16_t time)
{
	if (L_R == L || L_R == -L)
		SET_DUTY(-1000, 1000);
	else
		SET_DUTY(1000, -1000);

	if (Timer.start == 0)
	{
		Timer.start = millis();
		Timer.end = time;
	}
	else if (millis() - Timer.start >= Timer.end)
	{
		Timer_reset();
		Motion_next();
		return Motion;
	}

	if (port(L2) || port(R2))
	{
		Timer_reset();
		Motion_next();
	}
	else
		Motion.state = (L_R == L || L_R == -L) ? SPIN_TURN_L : SPIN_TURN_R;

	return Motion;
}