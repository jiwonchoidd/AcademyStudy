#include "TBitmapTmp.h"

bool		TBitmapTmp::Set(HDC hdc, DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(hdc);
	// ������ �޸𸮿� ��Ʈ�� ����
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
bool		TBitmapTmp::Draw( HDC hDC, RECT rt)
{
	StretchBlt(
		// ��ǥ ��� ����
		hDC, rt.left,rt.top,
		rt.right - rt.left,
		rt.bottom - rt.top,
		//// ��ǥ ��� ����
		//hDC, rt.right - rt.left,
		//rt.bottom - rt.top,
		//-(rt.right) + rt.left * 2,
		//-(rt.bottom) + rt.top * 2,

		// ���� ����
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
