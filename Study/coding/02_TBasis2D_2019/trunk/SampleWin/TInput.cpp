#include "TInput.h"
// KeyCheck(VK_LEFT)
// KeyCheck('0');
// KeyCheck('A');
DWORD TInput::KeyCheck(DWORD dwKey)
{
	// GetKeyState()		// 동기적
	// GetAsyncKeyState()	// 비동기적
	SHORT sKey = GetAsyncKeyState(dwKey);
	// 0x8000 =  10000000 00000000
	if (sKey & 0x8000)
	{
		if (m_dwKeyState[dwKey] == KEY_FREE)
			m_dwKeyState[dwKey] = KEY_PUSH;
		else
			m_dwKeyState[dwKey] = KEY_HOLD;
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_PUSH ||
			m_dwKeyState[dwKey] == KEY_HOLD  )
			m_dwKeyState[dwKey] = KEY_UP;
		else
			m_dwKeyState[dwKey] = KEY_FREE;
	}
	return m_dwKeyState[dwKey];
}
	// 클래스 멤버 변수 초기화
bool		TInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(m_dwMouseState, sizeof(DWORD) * 3);
	return true;
};
// 매 프레임에서 계산을 담당한다.
bool		TInput::Frame() 
{	
	return true;
};
// 매 프레임에서 드로우를 담당한다.
bool		TInput::Render() 
{
	ZeroMemory(m_csBuffer, sizeof(TCHAR)* MAX_PATH);
	m_strList.clear();
	/*for (int ikey = 0; ikey < 256; ikey++)
	{
		if (m_dwKeyState == KEY_FREE)
			strData += L"";
	}*/
	std::wstring strData0;
	if (m_dwMouseState[0] == TRUE)
	{
		strData0 = L"m_dwMouseState[0] ";
		m_strList.push_back(strData0);
	}
	
	std::wstring strData1;
	if (m_dwMouseState[1] == TRUE)
	{
		strData1 = L"m_dwMouseState[1] ";
		m_strList.push_back(strData1);
	}
	
	std::wstring strData2;
	if (m_dwMouseState[2] == TRUE)
	{
		strData2 = L"m_dwMouseState[2] ";
		m_strList.push_back(strData2);
	}	
	//_stprintf_s(m_csBuffer, L"%s\n", strData.c_str());
	return true;
};
// 클래스 멤버 변수 소멸을 담당한다.
bool		TInput::Release() 
{
	return true;
};
void TInput::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
		case WM_LBUTTONDOWN:
		{
			m_dwMouseState[0] = TRUE;
		}break;
		case WM_LBUTTONUP:
		{
			m_dwMouseState[0] = FALSE;
		}break;
		case WM_RBUTTONDOWN:
		{
			m_dwMouseState[1] = TRUE;
		}break;
		case WM_RBUTTONUP:
		{
			m_dwMouseState[1] = FALSE;
		}break;
		case WM_MBUTTONDOWN:
		{
			m_dwMouseState[2] = TRUE;
		}break;
		case WM_MBUTTONUP:
		{
			m_dwMouseState[2] = FALSE;
		}break;
	}
};
TInput::TInput()
{
}


TInput::~TInput()
{
}
