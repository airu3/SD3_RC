// Created by Tomoki Sato
//=======================================================================
// セットアップ
//=======================================================================
#pragma once

void setup()
{
#if defined(DEBUG_MODE)
	Serial.begin(2000000);
	delay(3000);
#endif

	// mode
	// INPUT:新ラジのサイドボタン
	// OUTPUT:モタドラ、ブザー、アーム用ソレノイドなど
	// INPUT_PULLUP:その他センサーなど

	// pinMode(L1, INPUT_PULLUP);
	// pinMode(R1, INPUT_PULLUP);
	// pinMode(L2, INPUT_PULLUP);
	// pinMode(R2, INPUT_PULLUP);
	// pinMode(FW, INPUT_PULLUP);
	// pinMode(L3, INPUT_PULLUP);
	// pinMode(R3, INPUT_PULLUP);

	// pinMode(lineL1, INPUT_PULLUP);
	// pinMode(lineR1, INPUT_PULLUP);
	// pinMode(ST_MODULE, INPUT_PULLUP); // 自立型 スタート・ストップモジュール
	// pinMode(FREE1, INPUT_PULLUP);
	// pinMode(FREE2, INPUT_PULLUP);
	pinMode(STICK1, INPUT_PULLUP);
	pinMode(STICK2, INPUT_PULLUP);
	pinMode(PB1, INPUT_PULLUP);
	pinMode(PB2, INPUT_PULLUP);
	pinMode(PB3, INPUT_PULLUP);
	pinMode(PB4, INPUT_PULLUP);

	// pinMode(ARM_SOLENOID, OUTPUT);
	//  pinMode(FREE3, INPUT_PULLUP);
	//  pinMode(FREE4, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT); // 28

	SET_STICK_DEAD_ZONE(30); // 1

	SET_ROBOT_TYPE(RC);					 // 2
	SET_CONTROLLER_TYPE(KO_MC8); // 3

	SET_MD_TYPE(OUT_DIR_4);		 // 4
	SET_MOTOR_RAMP(50);				 // 5
	SET_MOTOR_LIMIT(950);			 // 6
	SET_MOTOR_DIRECTION(1, 1); // 7
	SET_PWM_FREQ(30000);			 // 8

	SET_MULTI_PORT(STICK1);
	SET_MULTI_PORT(STICK2);
	// SET_MULTI_PORT(PB1);

	SOUND(0);
}

/*
	1 スティック不感帯(5~95)[%]

	2 ロボットタイプ(RC|AT)[ラジコン|自立][ラ自立はRCを指定]
	3 使用するコントローラー(TR_NRC|TR_MC8)[新ラジ|近藤科学]

	4 モータードライバの制御方式(dutyピンの数_方向指定ピンの数)
		OUT_DIR_4 = 1, // BTS7960 // モーター1つでduty指定と回転方向[0,1|1,0]を2ピンで指定する
		OUT2_DIR2,		 // 寺井さん基板 // モーター1つでduty指定が1ピン、回転方向[0|1]を1ピンで指定する
		OUT2_DIR4		 // 緑のヒートシンク基板 // モーター1つでduty指定が1ピン、回転方向[0,1|1,0]を2ピンで指定する
	5 dutyの変化をなめらかにする(1msあたりのduty変化値)[1~200]
	6 dutyの最大値の指定(100～1000)[duty]
	7 モーターの駆動反転(1|-1 , 1|-1)[正転|逆転,正転|逆転]
	8 モーターの駆動周波数(15000~30000)[Hz]
*/
