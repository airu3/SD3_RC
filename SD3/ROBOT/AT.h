// Created by Tomoki Sato
//=======================================================================
// 自立型
//=======================================================================
#pragma once

// 自立型の動作管理
//例 1. 通常動作を直進に設定する
// Robot.mode = ATTACK;
//例 2. 初動作を待ちに設定する
// Robot.tactics = SEARCH;
typedef struct
{
	int8_t tactics; // 初動作(くの字、左右後退など)
	int8_t mode;	 // 通常動作(直進、待ちなど)
} ROBOT;
ROBOT Robot;

// 戦闘状態
enum ROBOT_MODE
{
	ATTACK,
	SEARCH,
};
