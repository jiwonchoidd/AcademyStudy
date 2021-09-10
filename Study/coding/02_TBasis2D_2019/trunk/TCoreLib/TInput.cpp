#include "TInput.h"

TInputActionMap		g_ActionInput;
POINT				g_MousePos;
 //KeyCheck(VK_LEFT)VK_ESCAPE
// KeyCheck('0');
// KeyCheck('A');
// ���� : �ΰ��� ��Ʈ�߿� 1���� ���̸� ����� ���̵ȴ�.
// ���� : �ΰ��� ��Ʈ ��ΰ� �� �� ��츸 ����� ���̵ȴ�.
// ��Ÿ�� ���� : �ΰ��� ��Ʈ�� ���Ͽ� ���� �ٸ� ��쿡 ���̵ȴ�.
// ��Ÿ�� ���� : �Ѵ� ���� ��츸 ������ �ȴ�.
// ���� : �Ѱ��� ��Ʈ�� �ݴ�� ó���Ѵ�.( ��=>����, ����->��)
// ����, ����, ��Ÿ�� ����, ��Ÿ�� ����, ����
//   or     and      xor           xand          not
//   |       &       ^             ����          !   => ��Ʈ������
DWORD TInput::KeyCheck(DWORD dwKey)
{
	// GetKeyState()		// ������
	// GetAsyncKeyState()	// �񵿱���
	SHORT sKey = GetAsyncKeyState(dwKey);
	// 0x8000 =  10000000 00000000	
	if( (sKey & 0x8000) || (sKey & 0x8001) )
	{
		if (m_dwKeyState[dwKey] == KEY_FREE || m_dwKeyState[dwKey] == KEY_UP )
			m_dwKeyState[dwKey] = KEY_PUSH;
		else
			m_dwKeyState[dwKey] = KEY_HOLD;
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_PUSH || m_dwKeyState[dwKey] == KEY_HOLD  )
			m_dwKeyState[dwKey] = KEY_UP;
		else
			m_dwKeyState[dwKey] = KEY_FREE;
	}
	return m_dwKeyState[dwKey];
}
	// Ŭ���� ��� ���� �ʱ�ȭ
bool		TInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	return true;
};
// �� �����ӿ��� ����� ����Ѵ�.
bool		TInput::Frame() 
{	
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);	
	g_MousePos = m_MousePos;

	// ���� : GetAsyncKeyState�� ȣ������� Ű ���¸� ��ȯ�Ѵ�.
	// �ش� Ű�� Ŭ���Ǹ� ��ȯ��=0x8000�̴�. 
	// ������ GetAsyncKeyState ȣ�� ���� ���� �ش� Ű�� ������ ������
	// ���� ��ȯ��=0x8001 �� ��ȯ �ȴ�.(������ ���� ���� �ְ� ȣ������� ������ ����)
	g_ActionInput.bWKey = KeyCheck('W');
	g_ActionInput.bSKey = KeyCheck('S');
	g_ActionInput.bAKey = KeyCheck('A');
	g_ActionInput.bDKey = KeyCheck('D');
	g_ActionInput.bExit = KeyCheck(VK_ESCAPE);
	g_ActionInput.bSpace = KeyCheck(VK_SPACE);
	
	g_ActionInput.bLeftClick = KeyCheck(VK_LBUTTON);
	g_ActionInput.bMiddleClick = KeyCheck(VK_MBUTTON);
	g_ActionInput.bRightClick = KeyCheck(VK_RBUTTON);
	return true;
};
// �� �����ӿ��� ��ο츦 ����Ѵ�.
bool		TInput::Render() 
{
	m_strList.clear();

	std::wstring strData0;
	if (g_ActionInput.bLeftClick)
	{
		strData0 = L"m_dwMouseState[0] "+ std::to_wstring(g_ActionInput.bLeftClick);
		m_strList.push_back(strData0);
	}
	
	std::wstring strData1;
	if (g_ActionInput.bMiddleClick == TRUE)
	{
		strData1 = L"m_dwMouseState[1] " + std::to_wstring(g_ActionInput.bMiddleClick);
		m_strList.push_back(strData1);
	}
	
	std::wstring strData2;
	if (g_ActionInput.bRightClick == TRUE)
	{
		strData2 = L"m_dwMouseState[2] " + std::to_wstring(g_ActionInput.bRightClick);
		m_strList.push_back(strData2);
	}	
	return true;
};
// Ŭ���� ��� ���� �Ҹ��� ����Ѵ�.
bool		TInput::Release() 
{
	return true;
};
void TInput::MsgEvent(MSG msg)
{	
};
TInput::TInput()
{
}


TInput::~TInput()
{
}
