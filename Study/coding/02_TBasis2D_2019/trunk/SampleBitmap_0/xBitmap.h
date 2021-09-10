#pragma once
#include "TStd.h"

typedef struct tPOINT
{
	float  x;
	float  y;
} tPOINT, *tPPOINT;

#define NONE_DEFALU 0x00
#define LEFT_SCALE 0x01
#define RIGHT_SCALE 0x02

class xBitmap
{
public:
	HBITMAP		m_hBitmap;
	HDC			m_hMemDC;
	HBITMAP		m_hOldBitmap;
	BITMAP		m_BmpInfo;
	HDC			m_hScreenDC;
public:
	bool		Set(HDC hdc);
	bool		Load( TCHAR* pszLoadFile);
	bool		Load(DWORD  dwLoadID);
	bool		Draw(HDC hDC, 
		tPOINT pos,
		RECT rt, DWORD dwScale = 0 );
	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release();
public:
	xBitmap();
	virtual ~xBitmap();
};

