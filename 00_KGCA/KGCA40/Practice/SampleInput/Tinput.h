#pragma once
#include "TStd.h"
// 키보드, 마우스 ->윈도우API 사용
enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class Tinput : public TSingleton<Tinput>
{
	friend class TSingleton<Tinput>;
	//public:
	//	static Tinput* Get()
	//	{
	//		static Tinput gInput;
	//		return &gInput;
	//	}
private:
	DWORD	m_dwKeyState[256];
	POINT   m_ptPos;
public:
	DWORD   GetKey(DWORD dwKey);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	Tinput();
public:
	~Tinput();
};

#define g_Input Tinput::Get()