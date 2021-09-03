#include "TBitmap.h"
bool TBitmap::Load(const TCHAR* pszLoadFile)
{
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pszLoadFile, IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE |
		LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
bool TBitmap::Load(DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
void		TBitmap::Set(HDC hdc )
{
	m_hScreenDC = hdc;
};
bool		TBitmap::Draw(HDC hDC, tPOINT pos, RECT rt,
						DWORD dwScale,
						DWORD opMode )
{
	switch (dwScale)
	{
	case LR_ROTATION:
	{
		StretchBlt(
			// 목표 대상 영역
			hDC, pos.x+rt.right, pos.y,
			-(rt.right), rt.bottom,								  
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);
	}break;
	case TB_ROTATION:
	{
		StretchBlt(
			// 목표 대상 영역
			hDC, pos.x, pos.y+rt.bottom,
			rt.right, -(rt.bottom),//+pos.y*1.33f,
			// 원본 영역
			m_hMemDC,
			rt.left, rt.top, 
			rt.right, rt.bottom,
			opMode);
	}break;
	case LRTB_ROTATION:
	{
		StretchBlt(
			// 목표 대상 영역			
			hDC, pos.x + rt.right, 
				 pos.y + rt.bottom,
			-(rt.right), -(rt.bottom),			
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);

		//StretchBlt(
		//	// 목표 대상 영역
		//	hDC, rt.right - rt.left,
		//	rt.bottom - rt.top,
		//	-(rt.right) + rt.left * 2,
		//	-(rt.bottom) + rt.top * 2,
		//	// 원본 영역
		//	m_hMemDC, 0, 0, m_BmpInfo.bmWidth,
		//	m_BmpInfo.bmHeight,
		//	opMode);
	}break;
	default:
	{
		BitBlt(
			// 목표 대상 영역
			hDC, pos.x, pos.y,
			rt.right,
			rt.bottom,
			// 원본 영역
			m_hMemDC, rt.left, rt.top, opMode);
	}break;
	}
	
	return true;
};

bool		TBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
};

TBitmap::TBitmap()
{
}


TBitmap::~TBitmap()
{
}
