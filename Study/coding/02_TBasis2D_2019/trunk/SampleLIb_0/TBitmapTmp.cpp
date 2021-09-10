#include "TBitmapTmp.h"

bool		TBitmapTmp::Set(HDC hdc, DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(hdc);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
bool		TBitmapTmp::Draw( HDC hDC, RECT rt)
{
	StretchBlt(
		// 목표 대상 영역
		hDC, rt.left,rt.top,
		rt.right - rt.left,
		rt.bottom - rt.top,
		//// 목표 대상 영역
		//hDC, rt.right - rt.left,
		//rt.bottom - rt.top,
		//-(rt.right) + rt.left * 2,
		//-(rt.bottom) + rt.top * 2,

		// 원본 영역
		m_hMemDC, 0, 0, m_BmpInfo.bmWidth, 
						m_BmpInfo.bmHeight,
		SRCCOPY);
	return true;
};
bool		TBitmapTmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
};

TBitmapTmp::TBitmapTmp()
{
}


TBitmapTmp::~TBitmapTmp()
{
}
