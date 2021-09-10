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
	// �ʱ�ȭ
	bool		Init();
	// ���
	bool		Frame();
	// ��ο�, ������
	bool		Render();
	// ����
	bool		Release();
private:
	TInput(void);
	~TInput(void);
};
// Singleton ������ ����(���� �ν��Ͻ� ����)
#define I_Input TInput::GetInstance()
