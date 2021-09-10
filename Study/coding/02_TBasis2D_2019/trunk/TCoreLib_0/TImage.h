#pragma once
#include "TCoreStd.h"
class TImage
{
public:
	TCHAR_STRING m_strName;
	HDC			m_hMemDC;
	HBITMAP	    m_hBitmap;
	HBITMAP	    m_hOldBitmap;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

	bool Load( HWND hWnd, 
			 HDC hdc,
			 TCHAR_STRING fileName );
public:
	TImage(void);
	~TImage(void);
};

