#pragma once
#include "TStd.h"
//������ Ű���� ���콺��
//������ API�� �����

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

