// Created by Tomoki Sato
//=======================================================================
// タイマー
//=======================================================================
#pragma once

// タイマー
typedef struct
{
	uint32_t start;
	uint32_t end;
} TIMER;
TIMER Timer = {0};

// タイマーをリセットする
void Timer_reset()
{
	Timer = (TIMER){0};
}