#pragma once
#include "TCoreStd.h"
struct TInputState
{
	DWORD dwKeyW;
	DWORD dwKeyA;
	DWORD dwKeyS;
	DWORD dwKeyD;
	DWORD dwSpace;
};
enum keyState{
	KEY_FREE = 0,
	KEY_PUSH, 
	KEY_UP,
	KEY_HOLD,
};
class TInput : public TSingleton<TInput>
{
private:
	friend class TSingleton<TInput>;
public:
	TInputState  m_InputState;
	DWORD		 m_dwKeyState[256];
	int			 KeyCheck(DWORD dwCheckkey );
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
private:
	TInput(void);
	~TInput(void);
};
#define I_Input TInput::GetInstance()

