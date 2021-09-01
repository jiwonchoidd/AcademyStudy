#pragma once
#include "TCoreStd.h"

struct TInputState
{
	DWORD	dwKeyW;
	DWORD	dwKeyA;
	DWORD	dwKeyS;
	DWORD	dwKeyD;
	DWORD	dwSpace;
};

enum KeyState { KEY_FREE=0,
				KEY_PUSH,
				KEY_UP,
				KEY_HOLD, };
class TCOREDLL_API TInput
{	
public:
	static TInput m_UnionInput;	
	static TInput* GetInstance();
	TInputState m_InputState;
	DWORD		m_dwKeyState[256];
public:
	DWORD		KeyCheck( DWORD dwCheckKey);
	// 초기화
	bool		Init();
	// 계산
	bool		Frame();
	// 드로우, 랜더링
	bool		Render();
	// 삭제
	bool		Release();
private:
	TInput(void);
	~TInput(void);
};
// Singleton 디자인 패턴(단일 인스턴스 생성)
#define I_Input TInput::GetInstance()
