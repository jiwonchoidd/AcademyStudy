#include "TCore.h"
bool TCore::GameInit()
{
	m_Timer.Init();
	m_Input.Init();
	// 제 1의 버퍼 전면버퍼
	m_hScreenDC = GetDC(m_hWnd);
	// 제 2의 버퍼를 생성한다.
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);// 메모리
	m_hOffScreenBitmap = CreateCompatibleBitmap(
		m_hScreenDC,
		m_rtClient.right, 
		m_rtClient.bottom );
	// 반환값 = 이전 선택되어 있던 비트맵
	m_hOldBitmap = (HBITMAP)SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);
	
	Init();
	return true;
}
bool TCore::GameRelease()
{
	Release();
	SelectObject(m_hOffScreenDC, m_hOldBitmap);
	ReleaseDC(m_hWnd, m_hScreenDC);
	m_Timer.Release();
	m_Input.Release();
	return true;
}
bool TCore::GameFrame()
{
	m_Timer.Frame();
	m_Input.Frame();
	Frame();
	return true;
}
bool TCore::PreRender()
{
	// m_hOffScreenDC 를 지운다.
	COLORREF bkColor = RGB(255, 255, 255);
	HBRUSH hbrBack = CreateSolidBrush(bkColor);
	SelectObject(m_hOffScreenDC, hbrBack);
	PatBlt(m_hOffScreenDC, 0, 0,
		m_rtClient.right, m_rtClient.bottom,
		PATCOPY);
	return true;
}
bool TCore::PostRender()
{
	// m_hOffScreenDC 이미지를 m_hScreenDC로 복사해라.
	BitBlt(m_hScreenDC, m_rtClient.left, m_rtClient.top,
		m_rtClient.right, m_rtClient.bottom,
		m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
}
bool TCore::DrawDebug()
{
	RECT rect;
	if (m_hOffScreenDC != NULL)
	{
		GetClientRect(m_hWnd, &rect);
		SetBkColor(m_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(m_hOffScreenDC, RGB(0, 0, 255));
		SetBkMode(m_hOffScreenDC, TRANSPARENT);
		// 그리기
		DrawText(m_hOffScreenDC, m_Timer.m_csBuffer, -1,
			&rect, DT_LEFT | DT_VCENTER);
		//SetTextAlign(hdc, TA_LEFT);
		//TextOut(hdc, 100, 100, L"KGCA 게임아카데미",
		//	wcslen(L"KGCA 게임아카데미"));		
	}
	return true;
}
bool TCore::GameRender()
{
	PreRender();
		Render();
		m_Timer.Render();	
		m_Input.Render();
		DrawDebug();
	PostRender();
	return true;
}
bool TCore::GameRun()
{
	GameFrame();
	GameRender();
	return true;
};
void TCore::MsgEvent(MSG msg)
{
	m_Input.MsgEvent(msg);
};
TCore::TCore()
{
}


TCore::~TCore()
{
}
