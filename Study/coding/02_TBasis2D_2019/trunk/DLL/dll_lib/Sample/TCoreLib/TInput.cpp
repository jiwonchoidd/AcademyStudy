#include "TInput.h"

TInput TInput::m_UnionInput;

TInput* TInput::GetInstance()
{
	return &m_UnionInput;
};

// 초기화
bool TInput::Init()
{	
	memset(&m_dwKeyState, 0, sizeof(DWORD)*256 );
	return true;
};
DWORD TInput::KeyCheck( DWORD dwCheckKey)
{
	//GetKeyState();	// 동기
	//GetAsyncKeyState() // 비동기
	SHORT sKey = GetAsyncKeyState( dwCheckKey );
	//0x8000 = 10000000 00000000 // 눌러져
	//0x0000 = 00000000 00000000 // 안 눌러져
	if( sKey & 0x8000  )
	{
		if( m_dwKeyState[dwCheckKey] == KEY_FREE )
			m_dwKeyState[dwCheckKey] = KEY_PUSH;
		else
			m_dwKeyState[dwCheckKey] = KEY_HOLD;
	}
	else
	{
		if( m_dwKeyState[dwCheckKey] == KEY_PUSH ||
			m_dwKeyState[dwCheckKey] == KEY_HOLD ) 
			m_dwKeyState[dwCheckKey] = KEY_UP;
		else
			m_dwKeyState[dwCheckKey] = KEY_FREE;
	}
	return m_dwKeyState[dwCheckKey];
}
// 계산
bool TInput::Frame()
{
	m_InputState.dwKeyW = KeyCheck( 'W' );
	m_InputState.dwKeyS = KeyCheck( 'S' );
	m_InputState.dwKeyA = KeyCheck( 'A' );
	m_InputState.dwKeyD = KeyCheck( 'D' );
	return true;
};
// 드로우, 랜더링
bool TInput::Render()
{
	return true;
};
// 삭제
bool TInput::Release()
{
	return true;
};
TInput::TInput(void)
{
	
}


TInput::~TInput(void)
{
}
