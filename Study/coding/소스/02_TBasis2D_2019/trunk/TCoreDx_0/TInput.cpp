#include "TInput.h"
#include "TSys.h"
// KeyCheck(VK_LEFT)
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
	ZeroMemory(m_dwBeforeMouseState, sizeof(DWORD) * 3);
	return true;
};
// 매 프레임에서 계산을 담당한다.
bool		TInput::Frame() 
{	
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_dwBeforeMouseState[iButton] == KEY_PUSH )
		{
			if (m_dwMouseState[iButton] == KEY_PUSH)
			{
				m_dwMouseState[iButton] = KEY_HOLD;
			}
		}
		if (m_dwBeforeMouseState[iButton] == KEY_UP)
		{
			if (m_dwMouseState[iButton] == KEY_UP)
			{
				m_dwMouseState[iButton] = KEY_FREE;
			}
		}	
		m_dwBeforeMouseState[iButton] = m_dwMouseState[iButton];
	}	
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
		case WM_MOUSEWHEEL:
		{
			m_sMouseWheelDelta += (short)HIWORD(msg.wParam);
		}break;
		case WM_MOUSEMOVE:
		{
			m_MovePt.x = 0;
			m_MovePt.y = 0;
			if (GetCapture() == g_hWnd)
			{
				if (m_bDrag)
				{
					m_MovePt.x = msg.pt.x - m_DownPt.x;
					m_MovePt.y = msg.pt.y - m_DownPt.y;
					m_DownPt.x = msg.pt.x;
					m_DownPt.y = msg.pt.y;
				}
			}			
		}break;
		case WM_LBUTTONDOWN:
		{
			m_DownPt = msg.pt;			
			m_bDrag = true;
			m_dwMouseState[0] = KEY_PUSH;
			SetCapture(g_hWnd);
			OutputDebugString(L"down\n");
		}break;
		case WM_LBUTTONUP:
		{
			m_bDrag = false;
			m_UpPt = msg.pt;
			m_dwMouseState[0] = KEY_UP;		
			if (GetCapture() == g_hWnd)
			{
				ReleaseCapture();
			}
			OutputDebugString(L"up\n");
		}break;
		case WM_RBUTTONDOWN:
		{
			m_dwMouseState[1] = KEY_PUSH;
		}break;
		case WM_RBUTTONUP:
		{
			m_dwMouseState[1] = KEY_UP;
		}break;
		case WM_MBUTTONDOWN:
		{
			m_dwMouseState[2] = KEY_PUSH;
		}break;
		case WM_MBUTTONUP:
		{
			m_dwMouseState[2] = KEY_UP;
		}break;
	}
};
LRESULT TInput::MsgProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEWHEEL:
	{
		m_sMouseWheelDelta = (short)HIWORD(wParam);
	}break;
	case WM_MOUSEMOVE:
	{
		m_MovePt.x = 0;
		m_MovePt.y = 0;
		if (m_bDrag)
		{
			short iX = (short)LOWORD(lParam);
			short iY = (short)HIWORD(lParam);
			m_MovePt.x = iX - m_DownPt.x;
			m_MovePt.y = iY - m_DownPt.y;
			m_DownPt.x = iX;
			m_DownPt.y = iY;
			TCHAR pbuffer[256] = { 0, };
			_stprintf_s(pbuffer, L"\n%d %d",
				m_MovePt.x, m_MovePt.y);
			OutputDebugString(pbuffer);
		}
	}break;
	case WM_LBUTTONDOWN:
	{
		short iX = (short)LOWORD(lParam);
		short iY = (short)HIWORD(lParam);
		//POINTS Pt = MAKEPOINTS(lParam);
		m_DownPt.x = iX;
		m_DownPt.y = iY;
		m_bDrag = true;
		m_dwMouseState[0] = KEY_PUSH;
		OutputDebugString(L"down\n");
	}break;
	case WM_LBUTTONUP:
	{
		m_bDrag = false;
		//m_UpPt = msg.pt;
		short iX = (short)LOWORD(lParam);
		short iY = (short)HIWORD(lParam);
		m_UpPt.x = iX;
		m_UpPt.y = iY;
		m_dwMouseState[0] = KEY_UP;
		OutputDebugString(L"up\n");
	}break;
	case WM_RBUTTONDOWN:
	{
		m_dwMouseState[1] = KEY_PUSH;
	}break;
	case WM_RBUTTONUP:
	{
		m_dwMouseState[1] = KEY_UP;
	}break;
	case WM_MBUTTONDOWN:
	{
		m_dwMouseState[2] = KEY_PUSH;
	}break;
	case WM_MBUTTONUP:
	{
		m_dwMouseState[2] = KEY_UP;
	}break;
	}
	return 0;
}
TInput::TInput()
{
	m_DownPt.x = m_DownPt.y = 0;
	m_UpPt.x = m_UpPt.y = 0;
	m_MovePt.x = m_MovePt.y = 0;
	m_bDrag = false;
	m_sMouseWheelDelta = 0;
}


TInput::~TInput()
{
}
