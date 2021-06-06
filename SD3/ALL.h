// Created by Tomoki Sato
//=======================================================================
// インクルードファイル一覧
//=======================================================================
#pragma once

#if defined(DEBUG_MODE)
#define _DEBUG_ 1
#else
#define _DEBUG_ 0
#endif

typedef unsigned int uint;
typedef unsigned long ulong;

// 外部ライブラリ
// #include "Libraries/ae7seg.cpp"
#include "Libraries/fastestDigitalRW.hpp"
#include "Libraries/AVR_Timer.h"

#include "Assign.h"
#include "Config.h"
#include "GPIO.h"

#include "../Port.h"

#include "MMI/Buzzer.h"
#include "MMI/LED.h"
#include "MMI/Switch.h"
#include "MMI/MUSIC/Score.h"
#include "MMI/MUSIC/Player.h"

#include "MOTOR/Driver.h"
#include "MOTOR/Control.h"

#include "ROBOT/Timer.h"
#include "ROBOT/Motion.h"
#include "ROBOT/RC.h"
#include "ROBOT/AT.h"

#include "../Setup.h"
#include "../Movement.h"

/*
変数の型一覧
boolean	TrueまたはFalse

char	-128～127までの整数
unsigned char	0～255までの整数
uint8_t	0～255までの整数

int	-32768～32767までの整数
unsigned int	0～65535までの整数
word	0～65535までの整数

long	-2147483648～2147483647までの整数
unsigned long	0～4294967295までの整数

float	-3.4028235E+38～3.4028235E+38
double	-3.4028235E+38～3.4028235E+38

幅指定整数型変数の一覧
int8_t	8ビット(-128～127)の整数を格納
uint8_t	8ビット(0～255)を格納

int16_t	16ビット(-32768～32767)を格納
uint16_t	16ビット(0～65535)を格納

int32_t	32ビット(-2147483648～2147483647)を格納
uint32_t	32ビット(0～4294967295)を格納

int64_t	64ビット(-9223372036854775808～9223372036854775807)を格納
uint64_t	64ビット(0～18446744073709551615)を格納
*/