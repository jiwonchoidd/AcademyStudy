#include "Sample.h"

//// Ŭ���� ��� ���� �ʱ�ȭ
bool		Sample::Init()
{
	TCHAR  str[] = _T("NAME");
	TCHAR  str2[] = _T("NAME");
	_tcscpy(str, str2);
	return true;
};
//// �� �����ӿ��� ����� ����Ѵ�.
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
//// �� �����ӿ��� ��ο츦 ����Ѵ�.
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
//// Ŭ���� ��� ���� �Ҹ��� ����Ѵ�.
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
	// �����츦 �����Ѵ�.
	if (sample.SetWindow(hInstance, L"SampleWin",800, 600))
	{
		// ������ �����츦 �����Ѵ�.
		sample.Run();
	}
	//MessageBox(0, L"�����մϴ�.", L"������", MB_OK);
}