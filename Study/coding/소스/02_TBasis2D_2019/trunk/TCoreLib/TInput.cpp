#include "TInput.h"

TInputActionMap		g_ActionInput;
POINT				g_MousePos;
 //KeyCheck(VK_LEFT)VK_ESCAPE
// KeyCheck('0');
// KeyCheck('A');
// 논리합 : 두개의 비트중에 1개라도 참이면 결과는 참이된다.
// 논리곱 : 두개의 비트 모두가 참 일 경우만 결과는 참이된다.
// 배타적 논리합 : 두개의 비트를 비교하여 서로 다를 경우에 참이된다.
// 배타적 논리곱 : 둘다 참일 경우만 거짓이 된다.
// 부정 : 한개의 비트를 반대로 처리한다.( 참=>거짓, 거짓->참)
// 논리합, 논리곱, 배타적 논리합, 배타적 논리곱, 부정
//   or     and      xor           xand          not
//   |       &       ^             없음          !   => 비트연산자
DWORD TInput::KeyCheck(DWORD dwKey)
{
	// GetKeyState()		// 동기적
	// GetAsyncKeyState()	// 비동기적
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
	// 클래스 멤버 변수 초기화
bool		TInput::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	return true;
};
// 매 프레임에서 계산을 담당한다.
bool		TInput::Frame() 
{	
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);	
	g_MousePos = m_MousePos;

	// 주의 : GetAsyncKeyState는 호출시점의 키 상태를 반환한다.
	// 해당 키가 클릭되면 반환값=0x8000이다. 
	// 하지만 GetAsyncKeyState 호출 시점 전에 해당 키가 눌러져 있으면
	// 보통 반환값=0x8001 이 반환 된다.(이전에 누른 적이 있고 호출시점에 눌러져 있음)
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
// 매 프레임에서 드로우를 담당한다.
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
// 클래스 멤버 변수 소멸을 담당한다.
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
