#pragma once
#include "TBitmap.h"
class TObject
{
public:
	TBitmap		m_Bitmap;
	TBitmap		m_BitmapMask;
	bool		m_bMask;
	tPOINT		m_ObjPos; // ��ġ(��,����� ����)
	RECT		m_rtObj;  // �����̹������� ��ο��� ����
	RECT		m_rtCollision;  // �浹����(Ŭ���̾�Ʈ ��ǥ��)
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

