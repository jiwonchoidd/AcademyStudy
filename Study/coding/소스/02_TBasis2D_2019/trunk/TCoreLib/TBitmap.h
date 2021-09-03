#pragma once
#include "TStd.h"
#pragma comment (lib, "msimg32.lib") // AlphaBlend

typedef struct tPOINT
{
	float  x;
	float  y;
} tPOINT, *tPPOINT;

#define NONE_ROTATION 0x00
#define LEFT_ROTATION 0x01
#define RIGHT_ROTATION 0x02
#define ALPHA_DRAW 0x04

class TBitmap
{
public:
	std::wstring	m_szName;
	HBITMAP		m_hBitmap;
	HDC			m_hMemDC;
	HBITMAP		m_hOldBitmap;
	BITMAP		m_BmpInfo;
	HDC			m_hScreenDC;
	BLENDFUNCTION m_AlphaBlendFuction;
public:
	void		Set(HDC hdc);
	bool		Load(const TCHAR* pszLoadFile);
	bool		Load(DWORD  dwLoadID);
	bool		Draw(HDC hDC,
		tPOINT pos,
		RECT rt,
		DWORD dwRotate = 0,
		DWORD op = SRCCOPY,
		BLENDFUNCTION* pAlphaBF=0 );
	bool    Draw(float x, float y, RECT rt, DWORD iMode = SRCCOPY);
	bool    DrawColorKey(float x, float y, RECT rt, COLORREF color);
	bool		SaveFile(HBITMAP hBitmap, TCHAR* strSaveFile);
public:
	bool		Init() { return true; };
	bool		Frame() { return true; };
	bool		Render() { return true; };
	bool		Release();
public:
	TBitmap();
	virtual ~TBitmap();
};

