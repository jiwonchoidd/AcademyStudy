#include "Sample.h"

//// 클래스 멤버 변수 초기화
bool		Sample::Init()
{
	TCHAR  str[] = _T("NAME");
	TCHAR  str2[] = _T("NAME");
	_tcscpy(str, str2);
	return true;
};
//// 매 프레임에서 계산을 담당한다.
bool		Sample::Frame()
{
	if (m_Input.KeyCheck(VK_LEFT) == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"VK_LEFT", L"KEY", MB_OK);
	}
	if (m_Input.KeyCheck(VK_RIGHT) == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"VK_RIGHT", L"KEY", MB_OK);
	}
	if (m_Input.KeyCheck(VK_UP) == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"VK_UP", L"KEY", MB_OK);
	}
	if (m_Input.KeyCheck(VK_DOWN) == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"VK_DOWN", L"KEY", MB_OK);
	}
	if (m_Input.KeyCheck('0') == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"0", L"KEY", MB_OK);
	}
	if (m_Input.KeyCheck('A') == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"A", L"KEY", MB_OK);
	}
	if (m_Input.KeyCheck(VK_F1) == KEY_PUSH)
	{
		MessageBox(g_hWnd, L"VK_F1", L"KEY", MB_OK);
	}
	return true;
};
//// 매 프레임에서 드로우를 담당한다.
bool		Sample::Render()
{
	RECT rect;
	if (m_hOffScreenDC != NULL)
	{		
		SetBkColor(m_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(m_hOffScreenDC, RGB(0, 0, 255));
		SetBkMode(m_hOffScreenDC, TRANSPARENT);
		for (int icnt = 0; icnt < m_Input.m_strList.size();
				icnt++)
		{
			//SetTextAlign(hdc, TA_LEFT);
			TextOut(m_hOffScreenDC,
				0, 100+ icnt*20,
				m_Input.m_strList[icnt].c_str(),
				m_Input.m_strList[icnt].size());
		}
	}
	return true;
};
//// 클래스 멤버 변수 소멸을 담당한다.
//bool		Sample::Release()
//{
//	return true;
//};
Sample::Sample()
{
}


Sample::~Sample()
{
}
int WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	Sample sample;
	// 윈도우를 생성한다.
	if (sample.SetWindow(hInstance, L"SampleWin",800, 600))
	{
		// 생성된 윈도우를 가동한다.
		sample.Run();
	}
	//MessageBox(0, L"종료합니다.", L"윈도우", MB_OK);
}