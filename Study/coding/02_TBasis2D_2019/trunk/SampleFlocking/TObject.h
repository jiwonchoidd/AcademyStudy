#pragma once
#include "TBitmap.h"
class TObject
{
public:
	TBitmap		m_Bitmap;
	TBitmap		m_BitmapMask;
	bool		m_bMask;
	tPOINT		m_ObjPos; // 위치(좌,상단이 원점)
	RECT		m_rtObj;  // 원본이미지에서 드로우할 영역
	RECT		m_rtCollision;  // 충돌영역(클라이언트 좌표계)
	HDC			m_hOffScreenDC;
	HBRUSH		m_hBrush;
	HBRUSH		m_hOldBrush;
	DWORD		m_dwReverse;
	bool		m_bHero;
	int			m_iDirection[2];
	float		m_fSpeed;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	void		SetRect(RECT rt, 
						tPOINT pos, 
						bool bHero=false, 
						float fSpeed= 10.0f);
	bool		Load(	HDC hScreenDC, 
						HDC hOffScrenDC, 
						TCHAR* pszMaskLoadFileName,
						TCHAR* pszLoadFileName);
	void		Left();
	void		Right();
	void		Up();
	void		Down();
public:
	TObject();
	virtual ~TObject();
};

