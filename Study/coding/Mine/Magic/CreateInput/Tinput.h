#pragma once
#include "TStd.h"
//간단한 키보드 마우스는
//윈도우 API를 사용함

enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};

class Tinput : public TSingleton<Tinput>
{
	friend class TSingleton<Tinput>;
};

