// Created by Tomoki Sato
//=======================================================================
// ラ自立・自立型の複合動作
//=======================================================================
#pragma once

typedef struct EVENT_TIME
{
	// E2E end to end 土俵の端から端まで
	// half 仕切り線から土俵の端まで
	// line 白線から土俵に戻るまで

	int forward_E2E;
	int forward_half;

	int back_E2E;
	int back_half;
	int back_line;

	// 片輪旋回で指定角度になるまで
	int turn45;
	int turn90;

	// 超信地旋回で指定角度になるまで
	int spin_turn45;
	int spin_turn90;
} TIME;
TIME t;

// 戦術(フラグ処理に使用)
enum STATE
{
	// 複合動作
	BACK_TURN_L,
	BACK_TURN_R,
	KUNOJI_L,
	KUNOJI_R,
	FEINT_KNJ_L,
	FEINT_KNJ_R,
	AIM
};

// ラ自立・自立型共通
//----------------------------------------------------------------------

// 白線処理(旋回方向)[L|R]
/**
 * @brief 白線処理
 * @param L_R 左旋回か右旋回か
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION back_turn(int L_R)
{
	static MOTION m;

	switch (m.step)
	{
	case 0: // 後ろに下がる[ms]
		m = back(40);
		break;

	case 1: // 方向転換(左|右,旋回時間)[ms]
		m = spin_turn(L_R, 75);
		break;

	case 2: // 直進
		m = forward(40);
		break;

	default:
		m.step = 0;
		m.state = 0;
		return m;
	}

	m.state = (L_R == L) ? BACK_TURN_L : BACK_TURN_R;
	return m;
}

// くの字(攻める方向)[L|R]
/**
 * @brief くの字
 * @param L_R 左くの字か右くの字か
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION kunoji(int L_R)
{
	static MOTION m;
	m = Motion;

	switch (m.step)
	{
	case 0:			  // 最初の方向転換
		if (L_R == L) // 左くの字なら左旋回
			m = spin_turn(L, 40);
		else // 右くの字なら右旋回
			m = spin_turn(R, 40);
		break;

	case 1: // 向いた方向に直進
		m = forward(150);
		break;

	case 2:			  // 土俵の内側を向く
		if (L_R == L) // 右旋回
			m = spin_turn(R, 80);
		else // 左旋回
			m = spin_turn(L, 80);
		break;

	default: // 直進する(ボタンを離すまで繰り返す)
		SET_DUTY(1000, 1000);
		m.state = 0;
		return m;
	}
	m.state = (L_R == L) ? KUNOJI_L : KUNOJI_R;
	return m;
}

// くの字フェイント(攻める方向)[L|R]
/**
 * @brief くの字フェイント
 * @param L_R 左くの字フェイントか右くの字フェイントか
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION feint_kunoji(int L_R)
{
	static MOTION m;
	m = Motion;

	switch (m.step)
	{
		// 初動作で相手を誘う
	case 0: // 方向転換
		if (L_R == L)
			m = spin_turn(R, 20);
		else
			m = spin_turn(L, 20);
		break;

	case 1: // 少し前に進む
		m = forward(100);
		break;

	case 2: // 初期位置にもどる
		m = back(140);
		break;

		// 通常くの字
	case 3:
		if (L_R == L)
			m = spin_turn(L, 40);
		else
			m = spin_turn(R, 40);
		break;

	case 4:
		m = forward(100);
		break;

	case 5:
		if (L_R == L)
			m = spin_turn(R, 40);
		else
			m = spin_turn(L, 40);
		break;

	default: // それ以降:反応している間は6を繰り返す
		SET_DUTY(1000, 1000);
		m.state = 0;
		return m;
	}
	m.state = (L_R == L) ? FEINT_KNJ_L : FEINT_KNJ_R;
	return m;
}

// 後ろスティックの動作(旋回方向)[L|R]
/**
 * @brief 後ろスティックの動作
 * @param L_R 左旋回か右旋回か
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION turn_back(int L_R)
{
	static MOTION m;
	m = Motion;

	switch (m.step)
	{
	case 0: // 方向転換(左|右,旋回時間)[ms]
		m = spin_turn(L_R, 50);
		break;

	case 1: // 後ろに下がる[ms]
		m = back(125);
		break;

	default:
		SET_DUTY(0, 0);
		m.state = 0;
		return m;
	}
	m.state = BACK;
	return m;
}

// 停止機能付きのback(指定時間)[L|R]]
/**
 * @brief 停止機能付きのback
 * @param time 後ろに下がる時間[ms]
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION limit_back(int time)
{
	static MOTION m;
	m.step = Motion.step;

	switch (m.step)
	{
	case 0: // 後ろに下がる[ms]
		m = back(time);
		break;

	default: // 停止
		SET_DUTY(0, 0);
		m.state = 0;
		return m;
	}
	return m;
}

// セミオート旋回(左旋回|右旋回)[L|R]
/**
 * @brief セミオート旋回
 * @param L_R 左旋回か右旋回か
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION repeat_turn(int L_R)
{
	switch (Motion.step)
	{
	case 0:
		Motion = turn(L_R, t.turn90);
		break;

	case 1:
		Motion = forward(t.forward_half);
		break;

	default:
		Motion.step = 0;
		Motion.state = 0;
		return Motion;
	}
	return Motion;
}

/*//--自立型--//*/
//----------------------------------------------------------------------

// リトライ機能付きのSET_DUTY(m1,m2)[2秒経過でリトライ]
/**
 * @brief リトライ機能付きのSET_DUTY
 * @param m1 左モータのPWM
 * @param m2 右モータのPWM
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION multi_set_duty(int m1duty, int m2duty)
{
	static int cycle = 1;

	switch (Motion.step)
	{
	case 0:
		Motion = timer_set_duty(m1duty, m2duty, 2000);
		break;

	case 1:
		Motion = timer_set_duty(1000, 1000, 350);
		break;

	case 2:
		Motion = timer_set_duty(0, 0, 150);
		break;

	default:
		if (cycle > 4)
		{
			Motion.step = 0;
			cycle = 1;
		}
		else
		{
			Motion.step = 1;
			cycle++;
		}
	}
	return Motion;
}

//[動作周期内|周期終了][0|1] 間欠動作(周期)[ms]
/**
 * @brief 間欠動作
 * @param cycle_ms 周期[ms]
 * @return int 動作周期内なら0, 周期終了なら1
 */
bool gaga(int cycle_ms) // ｶﾞｶﾞ
{
	static unsigned long time;

	if (time == 0)
		time = millis();

	if (millis() - time <= 20)
	{
		SET_DUTY(1000, 1000);
	}
	else if (millis() - time <= cycle_ms)
	{
		SET_DUTY(0, 0);
	}
	else
	{
		t = {0};
		return 1;
	}

	return 0;
}

// 相手が動くまで間合いを詰める
/**
 * @brief 相手が動くまで間合いを詰める
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION aim()
{
	static MOTION m;
	static int cycle = 1;
	// 前方センサーが左右反応、斜めセンサが左右とも同じ状態、ガガの周期以下
	if ((port(L1) && port(R1)) && port(L2) == port(R2) && cycle <= 8)
	{
		cycle += gaga(1000);
		m.state = AIM;
	}
	else
		m.state = 0;

	return m;
}

// 時間経過で変化する旋回
/**
 * @brief 時間経過で変化する旋回
 * @param L_R 左旋回か右旋回か
 * @return MOTION 現在の動作と何ステップ目か
 */
MOTION multi_turn(int L_R)
{
	switch (Motion.step)
	{
	case 0:
		if (L_R == L)
			Motion = timer_set_duty(-250, 1000, 50);
		else
			Motion = timer_set_duty(1000, -250, 50);
		break;

	case 1:
		if (L_R == L)
			Motion = timer_set_duty(-500, 1000, 100);
		else
			Motion = timer_set_duty(1000, -500, 100);
		break;

	case 2:
		if (L_R == L)
			Motion = timer_set_duty(-1000, 1000, 200);
		else
			Motion = timer_set_duty(1000, -1000, 200);
		break;

	default:
		Motion.step = 1;
		Motion.state = 0;
	}
	return Motion;
}
