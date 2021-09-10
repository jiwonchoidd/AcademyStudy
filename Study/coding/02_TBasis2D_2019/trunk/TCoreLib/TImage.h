#pragma once
#include "TStd.h"
class TImage
{
public:
	T_STR		m_szPath;
	T_STR		m_szName;
	int			m_iIndex;
	float		m_fScale;
	HDC			m_hMemDC;
	HBITMAP	    m_hBitmap;
	HBITMAP	    m_hOldBitmap;
	BITMAP		m_bmpInfo;	
	LPBYTE		m_pRaster;
	BITMAPFILEHEADER	m_bitFileHeader;		
	BLENDFUNCTION m_BlendFunction;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	LoadDIB(const TCHAR* pFileName);
	bool    Load(std::wstring filename);
	bool	Load( HWND hWnd, HDC hdc, T_STR fileName );
	bool    Draw(float x, float y, RECT rt, DWORD iMode = SRCCOPY);
	bool    Draw(float x, float y, RECT rt, BLENDFUNCTION bf);
	bool    Draw(RECT rtDesk, RECT rtSrc, DWORD iMode);
	bool    DrawColorKey(float x, float y, RECT rt, COLORREF color);
	bool	CreateDIB(LONG iWidth, LONG iHeight, WORD bitCount=32);
public:
	TImage(void);
	~TImage(void);
};

