#pragma once
#include "TStd.h"

typedef struct tPOINT
{
	float  x;
	float  y;
} tPOINT, *tPPOINT;

#define NONE_ROTATION 0x00
#define LR_ROTATION 0x01
#define TB_ROTATION 0x02
#define LRTB_ROTATION 0x04
#define ALPHA_DRAW 0x08

class TBitmap
{
public:
	HBITMAP		m_hBitmap;
	HDC			m_hMemDC;
	HBITMAP		m_hOldBitmap;
	BITMAP		m_BmpInfo;
	HDC			m_hScreenDC;
public:
	void		Set(HDC hdc);
	bool		Load(const TCHAR* pszLoadFile);
	bool		Load(DWORD  dwLoadID);
	bool		Draw(	HDC hDC, 
						tPOINT pos, 	
						RECT rt, 						
						DWORD dwRotate = 0,
						DWORD op = SRCCOPY );
	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release();
public:
	TBitmap();
	virtual ~TBitmap();
};

