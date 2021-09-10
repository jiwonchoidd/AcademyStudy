#include "TInput.h"
int	 TInput::KeyCheck(DWORD dwCheckkey )
{
	if( GetAsyncKeyState(dwCheckkey) & 0x8000 )
	{
		if( m_dwKeyState[dwCheckkey] == KEY_FREE )
			m_dwKeyState[dwCheckkey] = KEY_PUSH;
		else
			m_dwKeyState[dwCheckkey] = KEY_HOLD;		
	}
	else
	{
		if( m_dwKeyState[dwCheckkey] == KEY_PUSH ||
			m_dwKeyState[dwCheckkey] == KEY_HOLD )
			m_dwKeyState[dwCheckkey] = KEY_UP;
		else
			m_dwKeyState[dwCheckkey] = KEY_FREE;
	}
	return m_dwKeyState[dwCheckkey];
}
bool TInput::Init()
{
	memset( &m_dwKeyState, 0, sizeof( DWORD ) * 256 );
	return true;
}
bool TInput::Frame()
{	
	m_InputState.dwKeyW = KeyCheck('W');
	m_InputState.dwKeyA = KeyCheck('A');
	m_InputState.dwKeyS = KeyCheck('S');
	m_InputState.dwKeyD = KeyCheck('D');
	m_InputState.dwSpace = KeyCheck(VK_SPACE);
	return true;
}
bool TInput::Render()
{
	return true;
}
bool TInput::Release()
{
	return true;
}
TInput::TInput(void)
{
}


TInput::~TInput(void)
{
}
