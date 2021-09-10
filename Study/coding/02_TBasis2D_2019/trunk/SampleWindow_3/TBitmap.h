#pragma once
#include "TStd.h"
class TBitmap
{
public:
	int		 m_iIndex;
	TCHAR    m_szName[MAX_PATH];
	HDC		 m_hScreenDC;
	HDC		 m_hOffScreenDC;
	HDC      m_hMemDC;
	HBITMAP  m_hBitmap;
	HBITMAP  m_hOldBitmap;
	BITMAP   m_hBitInfo;
public:
	// 전역 변수 선언시 사용됨.(HDC)
	bool     Load(TCHAR* pFileName);
	bool     Load( HDC hScreenDC, 
				   HDC hOffScreenDC,
				  TCHAR* pFileName);
	bool     Init();
	bool     Frame();
	bool     Render();
	bool     Release();
public:
	TBitmap();
	virtual ~TBitmap();
};

